#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

void Mission::init()
{
    while (globals_status == 0) globals_status = *reinterpret_cast<Globals_status**>(MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C); // Globals::status
}

int Mission::getid()
{
    return globals_status->m_nCurrentCampaignMission;
}

void Mission::setid(int value)
{
    globals_status->m_nCurrentCampaignMission = value;
}

int Mission::getcompletedsidemissions()
{
    return globals_status->m_nCompletedSideMissions;
}

void Mission::setcompletedsidemissions(int value)
{
    globals_status->m_nCompletedSideMissions = value;
}