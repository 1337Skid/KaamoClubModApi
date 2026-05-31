#ifndef IMGUIHANDLER_H
#define IMGUIHANDLER_H
#include <functional>
#include <vector>
#include "luamanager.h"
#include <string>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <windows.h>

struct LuaWindow {
    std::string title;
    std::function<void()> draw_callback;
    bool isopen = true;
    int togglekey = 0;
    bool lockinput = true;
};

class ImGuiHandler {
    public:
        static void init(LuaManager *luamanager);
        static void shutdown(void);
        static void add_window(const std::string& title, std::function<void()> callback, int togglekey, bool lockinput);
        static void set_windowopen(const std::string& title, bool isopen);
        static bool is_windowopen(const std::string& title);
        static bool isluawindowopen(void);
        static bool blockinput(void);
        static inline bool isinit = false;
        static inline HWND handle_hwnd = nullptr;
        static inline WNDPROC handle_wndproc = nullptr;
        static inline bool isshuttingdown = false;
};
#endif