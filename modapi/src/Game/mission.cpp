#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <thread>
#include <chrono>

void Mission::init()
{
    auto start = std::chrono::high_resolution_clock::now();
    uintptr_t base = MemoryUtils::GetModuleBase("GoF2.exe");
    
    while (globals_status == nullptr) {
        globals_status = *reinterpret_cast<Globals_status**>(base + 0x20AD6C); // Globals::status
        if (globals_status == nullptr)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("[+] Mission Initialization took: %lld ms\n", duration);
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