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
    uintptr_t address = Offset::STATUS_NEXTCAMPAIGNMISSION;
    Globals_status* ptr_status = (*globals_status);
    
    __asm {
        mov ecx, ptr_status
        call address
    }
}

// TODO: check if stationid etc.. are nulls (probs a lot of function needs that tbh)
// TODO: do type freelance
// TODO: planets does not show marker on MGame
// TODO: might crash if clicked on mission btn if in a correct stationid with the current mission (MGame) (pause check won't work - tested)
// TODO: if i open the map and if i get out of the station where the mission was i see no ships??
// TODO: Why the hell is real mission id incrementing LOL?
int Mission::create(int stationid, std::string description, int type)
{
    if (EventManager::isearlyinit_finished) {
        std::cout << "[-] Failed to call mission:Create(), you can only call it in the EarlyInit event" << std::endl;
        return -1;
    }
    CustomMission cm;
    int len = MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, nullptr, 0);
    std::wstring descriptionws(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, descriptionws.data(), len);
    cm.description = descriptionws;
    cm.stationid = stationid;
    cm.type = type;
    cm.enabled = 0;
    cm.entered_mission = 0;
    created_missions.push_back(cm);
    return created_missions.size();
}

// TODO: if a mission is already enabled then cancel it needs to be disabled
void Mission::enable(int custom_missionid)
{
    if (custom_missionid >= 0 && (custom_missionid - 1) < created_missions.size()) {
        created_missions[custom_missionid - 1].enabled = 1;
        return;
    }
    std::cout << "[-] Failed to enable custom mission id because the id doesn't exist anymore, did you call create?" << std::endl;
}

void Mission::disable(int custom_missionid)
{
    uintptr_t m = reinterpret_cast<uintptr_t>((*globals_status)->m_pMission);

    if (custom_missionid >= 0 && (custom_missionid - 1) < created_missions.size()) {
        created_missions[custom_missionid - 1].enabled = 0;
        *(int*)(m + 8) = -1; // disable
        *(int*)(m + 0x30) = 0;
        return;
    }
    std::cout << "[-] Failed to enable custom mission id because the id doesn't exist anymore, did you call create?" << std::endl;
}