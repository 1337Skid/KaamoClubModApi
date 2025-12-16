#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

class EventManager {
    private:
        static std::map<std::string, std::vector<sol::protected_function>> listeners;
        template <typename... Args>
        static void trigger(std::string eventname, Args&&... args)
        {
            if (listeners.find(eventname) == listeners.end())
                return;

            for (auto& func : listeners[eventname]) {
                auto result = func(std::forward<Args>(args)...);
                if (!result.valid()) {
                    sol::error err = result;
                    std::cerr << "[EventManager] Lua Error in event '" << eventname << "': " << err.what() << std::endl;
                }
            }
        }
        static void update_event(void);
        static void systemchanged_event(void);
        static void moneychanged_event(void);
        static void ingame_event(void);
        static void mainmenu_event(void);
    public:
        static void addlistener(std::string eventname, sol::protected_function callback);
        static void trigger_events(void);
        static void clearlisteners(void);
};
#endif