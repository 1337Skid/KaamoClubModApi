#include "hookcontext.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <windows.h>
#include "modapi_utils.h"
#include <Game/structs.h>
#include <Game/level.h>
#include <Game/ship.h>

void MissionContext::createfighter(int meshid, int faction)
{
    Level::createfighter(meshid, faction);
}

void MissionContext::createstaticobject(int type, float x, float y, float z)
{
    Level::createstaticobject(type, x, y, z);
}

int GlobalsInitContext::createship(const std::string& name, const std::string& description, sol::table shipinfo, int diffuse, int normal, int material, int lod0, int lod1, int lod2)
{
    return Ship::create(name, description, shipinfo, diffuse, normal, material, lod0, lod1, lod2);
}