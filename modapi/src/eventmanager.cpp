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

std::map<std::string, std::vector<sol::protected_function>> EventManager::listeners;

void EventManager::addlistener(std::string eventname, sol::protected_function callback)
{
    listeners[eventname].push_back(callback);
}

void EventManager::clearlisteners()
{
    listeners.clear();
}

void EventManager::ingame_event()
{
    // <1000 because when the game init the pointer it has some random values so the in game event gets triggered
    // TODO: Do a better ingame event impl because if we go in game and then go back to the main menu the pointer still has the mission id value
    if (Mission::getmissionid() > 0 && Mission::getmissionid() < 1000)
        trigger("IsInGame");
}

void EventManager::mainmenu_event()
{
    if (Mission::getmissionid() == 0)
        trigger("IsInMainMenu");
}

void EventManager::systemchanged_event()
{
    static int old = System::getsystemid();
    int current = System::getsystemid();

    if (current != old) {
        trigger("OnSystemChanged", current);
        old = current;
    }
}

void EventManager::moneychanged_event()
{
    static int old = Player::getmoney();
    int current = Player::getmoney();

    if (current != old) {
        trigger("OnMoneyChanged", current);
        old = current;
    }
}

void EventManager::update_event()
{
    trigger("OnUpdate");
}

void EventManager::trigger_events()
{
    update_event();
    systemchanged_event();
    moneychanged_event();
    mainmenu_event();
    ingame_event();
}