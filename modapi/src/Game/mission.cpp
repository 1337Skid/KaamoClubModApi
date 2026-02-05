#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include "patches.h"
#include "offset.h"

void Mission::init()
{
    globals_status = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
}

int Mission::getid()
{
    return (*globals_status)->m_nCurrentCampaignMission;
}

void Mission::setid(int value)
{
    (*globals_status)->m_nCurrentCampaignMission = value;
}

int Mission::getcompletedsidemissions()
{
    return (*globals_status)->m_nCompletedSideMissions;
}

void Mission::setcompletedsidemissions(int value)
{
    (*globals_status)->m_nCompletedSideMissions = value;
}

void Mission::enablevalkyrie()
{
    Patches::patchvalkyrie();
}

void Mission::nextcampaignmission()
{
    uintptr_t address = 0x004D605F;
    Globals_status* ptr_status = (*globals_status);
    __asm {
        mov ecx, ptr_status
        call address
    }
}