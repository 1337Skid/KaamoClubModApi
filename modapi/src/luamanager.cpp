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
#include <Game/structs.h>

void LuaManager::init()
{
    lua_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::os, sol::lib::io);
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
        "enemieskilled", sol::property(&Player::getenemieskilled, &Player::setenemieskilled),
        "level", sol::property(&Player::getlevel, &Player::setlevel),
        "visitedstations", sol::property(&Player::getvisitedstations, &Player::setvisitedstations),
        "cargosalvagedcount", sol::property(&Player::getcargosalvagedcount, &Player::setcargosalvagedcount),
        "asteroidsdestroyedcount", sol::property(&Player::getasteroidsdestroyedcount, &Player::setasteroidsdestroyedcount),
        "HasShipArmor", [](Player& self) -> bool {
            return Player::hasshiparmor();
        },
        "IsDocked", [](Player& self) -> bool {
            return Player::isdocked();
        },
        "GetShipInfo", [](Player& self) -> sol::table {
            return Player::getshipinfo();
        },
        "SetShipInfo", [](Player& self, sol::table shipinfo) {
            Player::setshipinfo(shipinfo);
        },
        "HasJumpDrive", [](Player& self) -> bool {
            return Player::hasjumpdrive();
        },
        "ToggleCloaking", [](Player& self) {
            Player::togglecloaking();
        },
        "SetPosition", [](Player& self, float x, float y, float z) {
            Player::setposition(x, y, z);
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
        "Create", [](System& self, const std::string& str, int x, int y, int z, int faction, int risk, int textureid, int linkedsystemid) {
            return System::create(str, x, y, z, faction, risk, textureid, linkedsystemid);
        },
        "IsVisible", [](System& self, int systemid) {
            return System::isvisible(systemid);
        },
        "SetVisible", [](System& self, int systemid, bool visible) {
            return System::setvisible(systemid, visible);
        }
    );

    lua_state.new_usertype<Mission>("Mission",
        sol::no_constructor,
        "id", sol::property(&Mission::getid, &Mission::setid),
        "completedsidemissions", sol::property(&Mission::getcompletedsidemissions, &Mission::setcompletedsidemissions),
        "EnableValkyrie", [](Mission& self) {
            Mission::enablevalkyrie();
        },
        "NextCampaignMission", [](Mission& self) {
            Mission::nextcampaignmission();
        }
    );

    lua_state.new_usertype<Station>("Station",
        sol::no_constructor,
        "id", sol::property(&Station::getid, &Station::setid),
        "name", sol::property(&Station::getname, &Station::setname),
        "level", sol::property(&Station::gettechlevel, &Station::settechlevel),
        "hangaritemscount", sol::property(&Station::gethangaritemscount, &Station::sethangaritemscount),
        "hangarshipscount", sol::property(&Station::gethangarshipscount, &Station::sethangarshipscount),
        "agentscount", sol::property(&Station::getagentscount, &Station::setagentscount),
        "IsVoid", [](Station& self) -> bool {
            return Station::isvoid();
        },
        "SetHangarShipInfo", [](Station& self, int id, sol::table shipinfo) {
            Station::sethangarshipinfo(id, shipinfo);
        },
        "Create", [](Station& self, const std::string& str, int techlevel, int textureid, int systemid) {
            return Station::create(str, techlevel, textureid, systemid);
        },
        "GetAgentName", [](Station& self, int id) {
            return Station::getagentname(id);
        },
        "GetAgentFaction", [](Station& self, int id) {
            return Station::getagentfaction(id);
        },
        "CreateAgent", [](Station& self, const std::string& name, int factiontype, int terranwoman, int hair, int eyes, int mouth, int armor, sol::table agentinfo) {
            Station::createagent(name, factiontype, terranwoman, hair, eyes, mouth, armor, agentinfo);
        },
        "RemoveHangarItem", [](Station& self, int id) {
            Station::removehangaritem(id);
        },
        "HasItemInHangar", [](Station& self, int id) -> bool {
            return Station::hasiteminhangar(id);
        }
    );

    lua_state.new_usertype<Asset>("Asset",
        sol::no_constructor,
        "GetAssetFilePath", [](Asset& self, unsigned int id) -> std::string {
            return Asset::getassetfilepath(id);
        },
        "SetAssetFilePath", [](Asset& self, unsigned int id, const std::string filepath) {
            Asset::setassetfilepath(id, filepath);
        },
        "GetText", [](Asset& self, int id) -> std::string {
            return Asset::gettext(id);
        }
    );

    lua_state.new_usertype<Item>("Item",
        sol::no_constructor,
        "Create", [](Asset& self, const std::string& name, const std::string& description, sol::table iteminfo) -> int {
            return Item::create(name, description, iteminfo);
        }
    );

    lua_state.new_usertype<Level>("Level",
        sol::no_constructor,
        "CreateRadioMessage", [](Level& self, const std::string& name, const std::string& content, sol::table imageinfo)  {
            Level::createradiomessage(name, content, imageinfo);
        }
    );

    lua_state.set_function("RegisterEvent", [&](std::string name, sol::protected_function callback) {
        EventManager::addlistener(name, callback);
    });

    lua_state["player"] = Player();
    lua_state["system"] = System();
    lua_state["mission"] = Mission();
    lua_state["station"] = Station();
    lua_state["asset"] = Asset();
    lua_state["item"] = Item();
    lua_state["level"] = Level();
}

void LuaManager::execute_script(const std::string& filepath)
{
    try {
        std::filesystem::path path(filepath);
        std::string scriptdir = path.parent_path().string();
        std::string cpath = lua_state["package"]["path"];

        lua_state["package"]["path"] = cpath + ";" + scriptdir + "/?.lua";
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

lua_State* LuaManager::getluastate()
{
    return lua_state.lua_state();
}