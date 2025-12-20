#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

void Mission::init()
{
    global_status = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C; // Globals::status
}

int Mission::getid()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x1B0});
    return MemoryUtils::Read<int>(finaladdr);
}

void Mission::setid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x1B0});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Mission::getcompletedsidemissions()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x18C});
    return MemoryUtils::Read<int>(finaladdr);
}

void Mission::setcompletedsidemission(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x18C});
    MemoryUtils::Write<int>(finaladdr, value);
}