#include "hookcontext.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <windows.h>
#include "modapi_utils.h"
#include <Game/structs.h>
#include <Game/level.h>

void MissionContext::createfighter(int meshid, int faction)
{
    Level::createfighter(meshid, faction);
}

void MissionContext::createstaticobject(int type, float x, float y, float z)
{
    Level::createstaticobject(type, x, y, z);
}