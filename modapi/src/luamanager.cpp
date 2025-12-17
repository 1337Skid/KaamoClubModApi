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

void LuaManager::init()
{
    lua_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math);
}

void LuaManager::bind_api()
{

    //lua_state.set_function("HelloWorld", &HelloWorld);
    
    // TODO: do a better wait because if a script uses wait() then every scripts waits...
    lua_state.set_function("wait", [](int seconds) { 
        Sleep(seconds * 1000); 
    });

    lua_state.new_usertype<Player>("Player",
        sol::no_constructor,
        "money", sol::property(&Player::getmoney, &Player::setmoney),
        "cargo", sol::property(&Player::getcargo, &Player::setcargo),
        "maxcargo", sol::property(&Player::getmaxcargo, &Player::setmaxcargo),
        "armor", sol::property(&Player::getshiparmor, &Player::setshiparmor),
        "maxhealth", sol::property(&Player::getmaxshiphealth, &Player::setmaxshiphealth),
        "enemieskilled", sol::property(&Player::getenemieskilled, &Player::setenemieskilled),
        "level", sol::property(&Player::getlevel, &Player::setlevel),
        "visitedstations", sol::property(&Player::getvisitedstations, &Player::setvisitedstations),
        "jumpgateusedcount", sol::property(&Player::getjumpgateusedcount, &Player::setjumpgateusedcount),
        "cargotookcount", sol::property(&Player::getcargotookcount, &Player::setcargotookcount),
        "HasShipArmor", [](Player& self) -> bool {
            return Player::hasshiparmor();
        }
    );

    lua_state.new_usertype<System>("System",
        sol::no_constructor,
        "id", sol::property(&System::getsystemid, &System::setsystemid),
        "risk", sol::property(&System::getsystemrisklevel, &System::setsystemrisklevel),
        "faction", sol::property(&System::getsystemfaction, &System::setsystemfaction),
        "jumpgatestationid", sol::property(&System::getsystemjumpgatestationid, &System::setsystemjumpgatestationid),
        "mapcoordinate_x", sol::property(&System::getsystemmapcoordinatex, &System::setsystemmapcoordinatex),
        "mapcoordinate_y", sol::property(&System::getsystemmapcoordinatey, &System::setsystemmapcoordinatey),
        "mapcoordinate_z", sol::property(&System::getsystemmapcoordinatez, &System::setsystemmapcoordinatez)
    );

    lua_state.new_usertype<Mission>("Mission",
        sol::no_constructor,
        "id", sol::property(&Mission::getmissionid, &Mission::setmissionid),
        "completedsidemissions", sol::property(&Mission::getcompletedsidemissions, &Mission::setcompletedsidemission)
    );

    lua_state.new_usertype<Station>("Station",
        sol::no_constructor,
        "id", sol::property(&Station::getstationid, &Station::setstationid),
        "name", sol::property(&Station::getstationname, &Station::setstationname),
        "level", sol::property(&Station::getstationtechlevel, &Station::setstationtechlevel)
    );

    lua_state.new_usertype<Asset>("Asset",
        sol::no_constructor,
        "GetAssetFilePath", [](Asset& self, unsigned int offset) -> std::string {
            return Asset::getassetfilepath(offset);
        },
        "SetAssetFilePath", [](Asset& self, unsigned int offset, const std::string filepath) {
            Asset::setassetfilepath(offset, filepath);
        }
    );

    lua_state.set_function("RegisterEvent", [&](std::string name, sol::protected_function callback) {
        EventManager::addlistener(name, callback);
    });

    lua_state["API_VERSION"] = "1.0";
    lua_state["player"] = Player();
    lua_state["system"] = System();
    lua_state["mission"] = Mission();
    lua_state["station"] = Station();
    lua_state["asset"] = Asset();
}

void LuaManager::execute_script(const std::string& filepath)
{
    try {
        auto result = lua_state.script_file(filepath);
        
        if (!result.valid()) {
            sol::error err = result;
            std::cout << "[LuaManager] Lua Script error: " << err.what() << std::endl;
        }
    }
    catch (const sol::error& e) {
        std::cout << "[LuaManager] Lua exception: " << e.what() << std::endl;
    }
}