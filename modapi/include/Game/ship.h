#ifndef SHIP_H
#define SHIP_H
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <MinHook.h>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "luamanager.h"
#include "abyssengine.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

class Ship {
    public:
        struct Vector {
            float x;
            float y;
            float z;
        };
        struct EnginesPos {
            Vector position;
            Vector intensity;
        };
        struct CustomShip {
            int id;
            int lod0;
            int lod1;
            int lod2;
            int maxhealth;
            int baseprice;
            int maxcargo;
            int armor;
            float handling;
            int primaryslots;
            int secondaryslots;
            int turretslots;
            int equipmentslots;
            int hangar_y;
            std::vector<Vector> primary_positions;
            std::vector<Vector> secondary_positions;
            std::vector<Vector> turret_positions;
            std::vector<EnginesPos> engines;
            std::wstring name;
            std::wstring description;
        };
        static inline std::vector<CustomShip> created_ships;
        static int create(const std::string& name, const std::string& description, sol::table shipinfo, int diffuse, int normal, int material, int lod0, int lod1, int lod2);
};
#endif