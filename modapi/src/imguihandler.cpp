#include "imguihandler.h"
#include "eventmanager.h"
#include "offset.h"
#include <windows.h>
#include <d3d9.h>
#include <dinput.h>
#include <MinHook.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
typedef HRESULT(WINAPI *EndScene_define)(IDirect3DDevice9*);
static EndScene_define old_endscene = nullptr;
typedef HRESULT(WINAPI *Reset_define)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
static Reset_define old_reset = nullptr;
static uintptr_t resetaddr = 0;
typedef SHORT(WINAPI *GetAsyncKeyState_define)(int);
static GetAsyncKeyState_define old_getasynckeystate = nullptr;
typedef SHORT(WINAPI *GetKeyState_define)(int);
static GetKeyState_define old_getkeystate = nullptr;
typedef BOOL(WINAPI *ClipCursor_define)(const RECT*);
static ClipCursor_define old_clipcursor = nullptr;
typedef HRESULT(WINAPI *GetDeviceState_define)(IDirectInputDevice8*, DWORD, LPVOID);
static GetDeviceState_define old_getdevicestate = nullptr;
typedef HRESULT(WINAPI *GetDeviceData_define)(IDirectInputDevice8*, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD);
static GetDeviceData_define old_getdevicedata = nullptr;
typedef int(WINAPI *ShowCursor_define)(BOOL);
static ShowCursor_define old_showcursor = nullptr;

static LuaManager *_luamanager = nullptr;
static std::vector<LuaWindow> windows;
static bool keywasdown[256] = {};

void ImGuiHandler::add_window(const std::string& title, std::function<void()> callback, int togglekey, bool lockinput)
{ 
    windows.push_back({ title, callback, true, togglekey, lockinput }); 
}

bool ImGuiHandler::isluawindowopen()
{
    return std::any_of(windows.begin(), windows.end(), [](const LuaWindow& w) { return w.isopen; });
}

bool ImGuiHandler::blockinput()
{
    if (handle_hwnd && GetForegroundWindow() != handle_hwnd)
        return false;
    return std::any_of(windows.begin(), windows.end(), [](const LuaWindow& w) { 
        return w.isopen && w.lockinput; 
    });
}

void ImGuiHandler::set_windowopen(const std::string& title, bool isopen)
{
    for (auto& w : windows) {
        if (w.title == title)
            w.isopen = isopen;
    }
}

bool ImGuiHandler::is_windowopen(const std::string& title)
{
    for (auto& w : windows) {
        if (w.title == title)
            return w.isopen;
    }
    return false;
}

SHORT WINAPI getasynckeystate_hook(int key)
{
    if (ImGuiHandler::blockinput())
        return 0;
    return old_getasynckeystate(key);
}

SHORT WINAPI getkeystate_hook(int key)
{
    if (ImGuiHandler::blockinput())
        return 0;
    return old_getkeystate(key);
}

BOOL WINAPI clipcursor_hook(const RECT *rect)
{
    if (ImGuiHandler::blockinput())
        return old_clipcursor(nullptr);
    return old_clipcursor(rect);
}

int WINAPI showcursor_hook(BOOL show)
{
    return old_showcursor(show);
}

HRESULT WINAPI getdevicestate_hook(IDirectInputDevice8 *device, DWORD size, LPVOID data)
{
    HRESULT hr = old_getdevicestate(device, size, data);
    if (SUCCEEDED(hr) && ImGuiHandler::blockinput())
        memset(data, 0, size);
    return hr;
}

HRESULT WINAPI getdevicedata_hook(IDirectInputDevice8 *device, DWORD size, LPDIDEVICEOBJECTDATA data, LPDWORD elements, DWORD flags)
{
    HRESULT hr = old_getdevicedata(device, size, data, elements, flags);
    if (SUCCEEDED(hr) && ImGuiHandler::blockinput() && elements)
        *elements = 0;
    return hr;
}

static LRESULT WINAPI wndproc_hook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    WNDPROC original_wndproc = ImGuiHandler::handle_wndproc ? ImGuiHandler::handle_wndproc : reinterpret_cast<WNDPROC>(Offset::WNDPROC_GOF2);

    if (ImGuiHandler::isshuttingdown)
        return CallWindowProcW(original_wndproc, hwnd, msg, wparam, lparam);
    if (!ImGuiHandler::isinit)
        return CallWindowProcW(original_wndproc, hwnd, msg, wparam, lparam);
    if (msg == WM_DESTROY || msg == WM_NCDESTROY) {
        ImGuiHandler::isshuttingdown = true;
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(original_wndproc));
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        ImGuiHandler::isinit = false;
        return CallWindowProcW(original_wndproc, hwnd, msg, wparam, lparam);
    }
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return 1;
    if (ImGuiHandler::blockinput()) {
        if (msg == WM_SETCURSOR) {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return 1;
        }
        switch (msg) {
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_RBUTTONDBLCLK:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_MBUTTONDBLCLK:
            case WM_MOUSEMOVE:
            case WM_MOUSEWHEEL:
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_CHAR:
                return 1;
        }
    }

    return CallWindowProcW(original_wndproc, hwnd, msg, wparam, lparam);
}

static HRESULT WINAPI reset_hook(IDirect3DDevice9 *dev, D3DPRESENT_PARAMETERS *params)
{
    if (!ImGuiHandler::isinit)
        return old_reset(dev, params);
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = old_reset(dev, params);
    if (SUCCEEDED(hr))
        ImGui_ImplDX9_CreateDeviceObjects();
    return hr;
}

static HRESULT WINAPI endscene_hook(IDirect3DDevice9 *dev)
{
    if (!old_getasynckeystate || ImGuiHandler::isshuttingdown)
        return old_endscene(dev);

    if (!ImGuiHandler::isinit) {
        D3DDEVICE_CREATION_PARAMETERS params{};
        dev->GetCreationParameters(&params);
        if (!ImGuiHandler::handle_hwnd)
            ImGuiHandler::handle_hwnd = params.hFocusWindow;
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(ImGuiHandler::handle_hwnd);
        ImGui_ImplDX9_Init(dev);
        ImGuiHandler::handle_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(ImGuiHandler::handle_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc_hook)));
        ImGuiHandler::isinit = true;
    }
    static auto last_tick = std::chrono::steady_clock::now();
    auto current_tick = std::chrono::steady_clock::now();
    std::chrono::duration<float> ticks = current_tick - last_tick;
    float dt = ticks.count();
    last_tick = current_tick;
    if (_luamanager) {
        _luamanager->update(dt); // LuaManager is here because I can't make a new thread with the lua update + the imgui due to of the lua bindings of imgui, mutex won't work here
        EventManager::trigger_events();
    }
    for (auto& win : windows) {
        if (win.togglekey <= 0 || win.togglekey >= 256)
            continue;
        bool isdown = (old_getasynckeystate(win.togglekey) & 0x8000) != 0;
        if (isdown && !keywasdown[win.togglekey])
            win.isopen = !win.isopen;
        keywasdown[win.togglekey] = isdown;
    }
    static bool cursor_shown = false;
    static int show_cursor_count = 0;
    bool open = ImGuiHandler::isluawindowopen();
    bool focus = (GetForegroundWindow() == ImGuiHandler::handle_hwnd);
    if (open && focus) {
        if (!cursor_shown) {
            show_cursor_count = 0;
            do {
                show_cursor_count++;
            } while (old_showcursor(TRUE) < 0);
            cursor_shown = true;
        }
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        if (ImGuiHandler::blockinput())
            old_clipcursor(nullptr);
    } else if (cursor_shown) {
        for (int i = 0; i < show_cursor_count; i++)
            old_showcursor(FALSE);
        show_cursor_count = 0;
        cursor_shown = false;
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::GetIO().MouseDrawCursor = false;
    for (auto& win : windows) {
        if (!win.isopen)
            continue;
        if (ImGui::Begin(win.title.c_str(), &win.isopen)) {
            if (win.draw_callback) win.draw_callback();
        }
        ImGui::End();
    }
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    return old_endscene(dev);
}

static uintptr_t get_endscene()
{
    HWND tmp = CreateWindowA("STATIC", "tmp", WS_POPUP, 0, 0, 1, 1, NULL, NULL, NULL, NULL); // STATIC is mandatory to tell windows we want a dummy hwnd
    IDirect3D9 *d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) {
        DestroyWindow(tmp);
        return 0;
    }
    D3DPRESENT_PARAMETERS params{};
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = tmp;
    params.Windowed = TRUE;
    IDirect3DDevice9 *dev = nullptr;
    uintptr_t addr = 0;
    if (SUCCEEDED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, tmp, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &params, &dev)) && dev) {
        addr = (*reinterpret_cast<uintptr_t**>(dev))[42];
        resetaddr = (*reinterpret_cast<uintptr_t**>(dev))[16];
        dev->Release();
    }
    d3d->Release();
    DestroyWindow(tmp);
    return addr;
}

void ImGuiHandler::init(LuaManager *luamanager)
{
    _luamanager = luamanager;
    uintptr_t addr = get_endscene();
    IDirectInput8 *dinput = nullptr;
    if (SUCCEEDED(DirectInput8Create(GetModuleHandleA(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&dinput), nullptr))) {
        IDirectInputDevice8 *dev = nullptr;
        if (SUCCEEDED(dinput->CreateDevice(GUID_SysKeyboard, &dev, nullptr))) {
            void **vtable = *reinterpret_cast<void***>(dev);
            MH_CreateHook(vtable[9], reinterpret_cast<void*>(getdevicestate_hook), reinterpret_cast<void**>(&old_getdevicestate));
            MH_CreateHook(vtable[10], reinterpret_cast<void*>(getdevicedata_hook), reinterpret_cast<void**>(&old_getdevicedata));
            dev->Release();
        }
        if (SUCCEEDED(dinput->CreateDevice(GUID_SysMouse, &dev, nullptr))) {
            void **vtable = *reinterpret_cast<void***>(dev);
            MH_CreateHook(vtable[9], reinterpret_cast<void*>(getdevicestate_hook), reinterpret_cast<void**>(&old_getdevicestate));
            MH_CreateHook(vtable[10], reinterpret_cast<void*>(getdevicedata_hook), reinterpret_cast<void**>(&old_getdevicedata));
            dev->Release();
        }
        dinput->Release();
    }
    auto user32module = GetModuleHandleA("user32.dll");
    MH_CreateHook(GetProcAddress(user32module, "ClipCursor"), reinterpret_cast<void*>(clipcursor_hook), reinterpret_cast<void**>(&old_clipcursor));
    MH_CreateHook(GetProcAddress(user32module, "GetAsyncKeyState"), reinterpret_cast<void*>(getasynckeystate_hook), reinterpret_cast<void**>(&old_getasynckeystate));
    MH_CreateHook(GetProcAddress(user32module, "GetKeyState"), reinterpret_cast<void*>(getkeystate_hook), reinterpret_cast<void**>(&old_getkeystate));
    MH_CreateHook(GetProcAddress(user32module, "ShowCursor"), reinterpret_cast<void*>(showcursor_hook), reinterpret_cast<void**>(&old_showcursor));
    MH_CreateHook(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(endscene_hook), reinterpret_cast<void**>(&old_endscene));
    MH_CreateHook(reinterpret_cast<void*>(resetaddr), reinterpret_cast<void*>(reset_hook), reinterpret_cast<void**>(&old_reset));
    MH_EnableHook(MH_ALL_HOOKS);
}

void ImGuiHandler::shutdown()
{
    if (handle_hwnd && IsWindow(handle_hwnd)) {
        WNDPROC original_wndproc = handle_wndproc ? handle_wndproc : reinterpret_cast<WNDPROC>(Offset::WNDPROC_GOF2);
        SetWindowLongPtrW(handle_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(original_wndproc));
    }
    if (isinit) {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
    isinit = false;
    isshuttingdown = false;
}