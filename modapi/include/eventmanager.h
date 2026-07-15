#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <windows.h>
#include <vector>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "hookcontext.h"
#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

struct LuaHook {
    std::string event_name;
    sol::protected_function callback;
};

class EventManager {
    private:
        static std::map<std::string, std::vector<sol::protected_function>> listeners;
        static std::map<std::string, std::vector<LuaHook>> hooks;
        static inline Globals_appManager** globals_appmanager = 0;
        
        template <typename... Args>
        static void trigger(std::string eventname, Args&&... args)
        {
            if (listeners.find(eventname) == listeners.end())
                return;
            for (auto& func : listeners[eventname]) {
                // basically the async events
                if (eventname != "EarlyInit") {
                    sol::state_view lua(lua_manager->getluastate());
                    sol::thread thread = sol::thread::create(lua);
                    sol::coroutine cor(thread.state(), func);
                    std::vector<sol::object> stored_args;
                    (stored_args.push_back(sol::make_object(lua, args)), ...);
                    auto result = cor(std::forward<Args>(args)...);
                    lua_manager->handle_coroutine(std::move(thread), std::move(cor), result, nullptr, std::move(stored_args));
                } else {
                    auto result = func(std::forward<Args>(args)...);
                    if (!result.valid()) {
                        sol::error err = result;
                        std::cerr << "[EventManager] Lua Error in event '" << eventname << "': " << err.what() << std::endl;
                    }
                }
            }
        }
        static void update_event();
        static void systemchanged_event();
        static void moneychanged_event();
        static void ingame_event();
        static void mainmenu_event();
        static void stationchanged_event();
        static void stationdocked_event();
        static void enemiekilled_event();
        static void cargochanged_event();
        static void asteroiddestroyed_event();
        static void joingame_event();
        static void radarscan_event();
        static void onstationundocked_event();
    public:
        static inline bool isearlyinit_finished = false;
        static LuaManager* lua_manager;
        static void addlistener(std::string eventname, sol::protected_function callback);
        static void addhook(std::string hookname, sol::protected_function callback);
        static void earlyinit_event();
        static void trigger_events();
        static void clearlisteners();
        
        template<typename TContext>
        static TContext trigger_hook(const std::string &hookname)
        {
            TContext ctx;
            trigger_hook<TContext>(hookname, ctx);
            return ctx;
        }

        template<typename TContext>
        static void trigger_hook(const std::string &hookname, TContext &ctx)
        {
            ctx.call_original = true;
            if (hooks.find(hookname) == hooks.end())
                return;
            ctx.call_original = false;
            for (auto& hook : hooks[hookname]) {
                sol::state_view lua(lua_manager->getluastate());
                sol::thread thread = sol::thread::create(lua);
                sol::coroutine cor(thread.state(), hook.callback);
                std::vector<sol::object> stored_args;
                stored_args.push_back(sol::make_object(lua, &ctx));
                auto result = cor(sol::as_args(stored_args));
                lua_manager->handle_coroutine(std::move(thread), std::move(cor), result, nullptr, std::move(stored_args));
            }
        }
};
#endif