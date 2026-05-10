#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <sol/sol.hpp>
#include <vector>
#include <string>

class LuaManager {
    private:
        struct LuaTask {
            sol::thread script_thread;
            sol::coroutine cor;
            float time_left;
            std::function<void()> on_complete;
            std::vector<sol::object> args;
        };
        struct LuaRoute {
            int* ptr = nullptr;
        };
        sol::state lua_state;
        std::vector<LuaTask> tasks;
    public:
        void init();
        void bind_api();
        void execute_script(const std::string& filepath);
        void update(float dt);
        void handle_coroutine(sol::thread script, sol::coroutine cor, const sol::protected_function_result& result, std::function<void()> on_complete, std::vector<sol::object> args);
        lua_State* getluastate();
};

#endif