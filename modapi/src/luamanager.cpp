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
#include <Game/structs.h>

void LuaManager::init()
{
    lua_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::os);
}

void LuaManager::bind_api()
{    
    // TODO: do a better wait (aka a scheduler) because if a script uses wait() then every scripts waits...
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
        },
        "IsDocked", [](Player& self) -> bool {
            return Player::isdocked();
        }
    );

    lua_state.new_usertype<System>("System",
        sol::no_constructor,
        "id", sol::property(&System::getid, &System::setid),
        "name", sol::property(&System::getname, &System::setname),
        "risk", sol::property(&System::getrisklevel, &System::setrisklevel),
        "faction", sol::property(&System::getfaction, &System::setfaction),
        "jumpgatestationid", sol::property(&System::getjumpgatestationid, &System::setjumpgatestationid),
        "mapcoordinate_x", sol::property(&System::getmapcoordinatex, &System::setmapcoordinatex),
        "mapcoordinate_y", sol::property(&System::getmapcoordinatey, &System::setmapcoordinatey),
        "mapcoordinate_z", sol::property(&System::getmapcoordinatez, &System::setmapcoordinatez),
        "Create", [](System& self, const std::string& str, int x, int y, int z) {
            System::create(str, x, y, z);
        }
    );

    lua_state.new_usertype<Mission>("Mission",
        sol::no_constructor,
        "id", sol::property(&Mission::getid, &Mission::setid),
        "completedsidemissions", sol::property(&Mission::getcompletedsidemissions, &Mission::setcompletedsidemission)
    );

    lua_state.new_usertype<Station>("Station",
        sol::no_constructor,
        "id", sol::property(&Station::getid, &Station::setid),
        "name", sol::property(&Station::getname, &Station::setname),
        "level", sol::property(&Station::gettechlevel, &Station::settechlevel),
        "IsVoid", [](Station& self) -> bool {
            return Station::isvoid();
        }
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