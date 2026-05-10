#include "luamanager.h"
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
#include "memoryutils.h"
#include "eventmanager.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/level.h>
#include "offset.h"

std::map<std::string, std::vector<sol::protected_function>> EventManager::listeners;
std::map<std::string, std::vector<LuaHook>> EventManager::hooks;
LuaManager* EventManager::lua_manager = nullptr;

void EventManager::addhook(std::string hookname, sol::protected_function callback)
{
    hooks[hookname].push_back(LuaHook{hookname, callback});
}

void EventManager::addlistener(std::string eventname, sol::protected_function callback)
{
    listeners[eventname].push_back(callback);
}

void EventManager::clearlisteners()
{
    listeners.clear();
}

void EventManager::joingame_event()
{
    static bool joinedgame = false;

    if (!joinedgame && globals_appmanager && ((*globals_appmanager)->m_nCurrentModule == 5 || (*globals_appmanager)->m_nCurrentModule == 2)) {
        Item::refreshitemsprices();
        Level::created_radiomessages.clear(); // TODO: clean up messages by hooking ~RadioMessages() destructor
        Level::created_dialoguemessages.clear(); // TODO: clean up messages by TRYING HARDER! yeah I tried to clean up the messages with the dtor and it didn't work so look at vtables?.. (edit i found out why i was dumb anyway i should do it..)
        Level::created_cutscenepts.clear();
        joinedgame = true;
        trigger("OnJoinGame");
    }
    if (joinedgame && (*globals_appmanager)->m_nCurrentModule == 1)
        joinedgame = false;
}

void EventManager::ingame_event()
{
    if (globals_appmanager && ((*globals_appmanager)->m_nCurrentModule == 5 || (*globals_appmanager)->m_nCurrentModule == 2))
        trigger("IsInGame");
}

void EventManager::earlyinit_event()
{
    trigger("EarlyInit");
    globals_appmanager = reinterpret_cast<Globals_appManager**>(Offset::GLOBALS_APPMANAGER);
    isearlyinit_finished = true;
}

void EventManager::mainmenu_event()
{
    if (Mission::getid() == 0)
        trigger("IsInMainMenu");
}

void EventManager::stationchanged_event()
{
    static int old = Station::getid();
    int current = Station::getid();

    if (current != old) {
        Level::created_radiomessages.clear();
        Level::created_dialoguemessages.clear();
        Level::created_cutscenepts.clear();
        Globals_status** globals_status = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
        uintptr_t m = reinterpret_cast<uintptr_t>((*globals_status)->m_pMission);
        for (auto& custom_mission : Mission::created_missions) {
            if (custom_mission.enabled && current != custom_mission.stationid) {
                custom_mission.entered_mission = 0;
                break;
            }
            if (custom_mission.enabled && current == custom_mission.stationid) {
                custom_mission.entered_mission = 1;
                *(int*)(m + 8) = 1;
                *(int*)(m + 0x30) = custom_mission.stationid;
                break;
            }
        }
        trigger("OnStationChanged", current);
        old = current;
    }
}

void EventManager::systemchanged_event()
{
    static int old = System::getid();
    int current = System::getid();

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

void EventManager::enemiekilled_event()
{
    static int old = Player::getenemieskilled();
    int current = Player::getenemieskilled();

    if (current != old) {
        if (Player::isdocked())
            return;
        trigger("OnEnemieKilled", current);
        old = current;
    }
}

void EventManager::cargochanged_event()
{
    static int old = Player::getshipinfo()["cargo"];
    int current = Player::getshipinfo()["cargo"];

    if (current != old) {
        trigger("OnCargoChanged", current);
        old = current;
    }
}

void EventManager::asteroiddestroyed_event()
{
    static int old = Player::getasteroidsdestroyedcount();
    int current = Player::getasteroidsdestroyedcount();

    if (current != old) {
        trigger("OnAsteroidDestroyed", current);
        old = current;
    }
}

void EventManager::stationdocked_event()
{
    static bool isdocked = false;

    if (Player::isdocked() && !isdocked) {
        trigger("OnStationDocked");
        isdocked = true;
    }
    if (!Player::isdocked() && isdocked)
        isdocked = false;
}

void EventManager::update_event()
{
    trigger("OnUpdate");
}

void EventManager::trigger_events()
{
    update_event();
    ingame_event();
    if (globals_appmanager && ((*globals_appmanager)->m_nCurrentModule == 5 || (*globals_appmanager)->m_nCurrentModule == 2)) {
        systemchanged_event();
        moneychanged_event();
        stationchanged_event();
        stationdocked_event();
        enemiekilled_event();
        cargochanged_event();
        asteroiddestroyed_event();
    }
    joingame_event();
    mainmenu_event();
}