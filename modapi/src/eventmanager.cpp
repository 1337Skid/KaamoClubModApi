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
#include <Game/level.h>

std::map<std::string, std::vector<sol::protected_function>> EventManager::listeners;

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
    static Globals_appManager** globals_appmanager = reinterpret_cast<Globals_appManager**>(0x0060AEFC);
    static bool joinedgame = false;

    if (!joinedgame && (*globals_appmanager)->m_nCurrentModule != 1 && (*globals_appmanager)->m_nCurrentModule != 0) {
        Item::refreshitemsprices();
        joinedgame = true;
        trigger("OnJoinGame");
    }
    if (joinedgame && (*globals_appmanager)->m_nCurrentModule == 1)
        joinedgame = false;
}

void EventManager::ingame_event()
{
    static Globals_appManager** globals_appmanager = reinterpret_cast<Globals_appManager**>(0x0060AEFC);
    if (globals_appmanager && (*globals_appmanager)->m_nCurrentModule != 1 && (*globals_appmanager)->m_nCurrentModule != 0)
        trigger("IsInGame");
}

void EventManager::earlyinit_event()
{
    trigger("EarlyInit");
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
        trigger("OnStationChanged", current);
        old = current;
    }
}

void EventManager::systemchanged_event()
{
    static int old = System::getid();
    int current = System::getid();

    if (current != old) {
        Level::created_radiomessages.clear();
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
    // TODO: clean globals_appmanager bs and put it in a header
    static Globals_appManager** globals_appmanager = reinterpret_cast<Globals_appManager**>(0x0060AEFC);
    update_event();
    ingame_event();
    if (globals_appmanager && (*globals_appmanager)->m_nCurrentModule != 1 && (*globals_appmanager)->m_nCurrentModule != 0) {
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