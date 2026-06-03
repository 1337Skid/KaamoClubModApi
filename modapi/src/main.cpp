#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include "patches.h"
#include "hooks.h"
#include "imguihandler.h"
#include <Game/system.h>
#include <Game/level.h>
#include <chrono>

DWORD WINAPI MainThread(LPVOID lpParam)
{
    // TODO: CLEAN THE GOD DAMN HEADERS!!!!!!!!!!!!!
    HWND gamehwnd = nullptr;
    LuaManager *luamanager = new LuaManager();
    FILE* dummyfile;
    freopen_s(&dummyfile, "CONOUT$", "w", stdout);
    freopen_s(&dummyfile, "CONOUT$", "w", stderr);
    freopen_s(&dummyfile, "CONIN$", "r", stdin);
    
    std::cout << "[+] KaamoClubModAPI Loaded! | Version: 1.0.3" << std::endl;    
    luamanager->init();
    luamanager->bind_api();
    EventManager::lua_manager = luamanager;
    ModApiUtils::load_mods(luamanager);
    EventManager::earlyinit_event();
    Hooks::init();
    Player::init(luamanager->getluastate());
    System::init();
    Station::init();
    Mission::init();
    Asset::init();
    Item::init();
    //Ship::init();
    Level::init(luamanager->getluastate());
    ImGuiHandler::init(luamanager);
    SetUnhandledExceptionFilter(ModApiUtils::crashhandler);
    // a thread waiting to be closed
    while (true) {
        if (!gamehwnd)
            gamehwnd = FindWindowA("GOF2", nullptr);
        if (ImGuiHandler::isinit && ImGuiHandler::handle_hwnd)
            gamehwnd = ImGuiHandler::handle_hwnd;
        if (ImGuiHandler::isshuttingdown)
            break;
        if (gamehwnd && !IsWindow(gamehwnd))
            break;
        Sleep(10);
    }
    MH_DisableHook(MH_ALL_HOOKS);
    ImGuiHandler::shutdown();
    MH_Uninitialize();
    if (dummyfile)
        fclose(dummyfile);
    delete luamanager;
    ExitProcess(0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
        if (hThread)
            CloseHandle(hThread);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}