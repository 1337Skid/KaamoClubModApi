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

// TODO: Check if we are in the game for most of the events because there is the same bug as the ennemie killed event

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
    // Might now use the appmanager module to check if we are ingame like the ondock event
    if (Mission::getid() > 0 && Mission::getid() < 1000)
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
    // TODO: Fix the event being triggered one time when we get out of a station
    // why is it doing that? bcz the game init killed at 0 and static int old is at 0
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
    static int old = Player::getcargo();
    int current = Player::getcargo();

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
    if (isearlyinit_finished) {
        update_event();
        systemchanged_event();
        moneychanged_event();
        mainmenu_event();
        ingame_event();
        stationchanged_event();
        stationdocked_event();
        enemiekilled_event();
        cargochanged_event();
        asteroiddestroyed_event();
    }
}