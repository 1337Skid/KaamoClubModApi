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

void Station::init()
{
    globals_status = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C; // Globals::status
}

int Station::getid()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x160, 0x8});
    return MemoryUtils::Read<int>(finaladdr);
}

void Station::setid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x160, 0x8});
    MemoryUtils::Write<int>(finaladdr, value);
}

std::string Station::getname()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x160, 0x0, 0x0});
    return MemoryUtils::ReadWideString(finaladdr);
}

void Station::setname(const std::string value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x160, 0x0, 0x0});
    MemoryUtils::WriteWideString(finaladdr, value);
}

int Station::gettechlevel()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x160, 0x1C});
    return MemoryUtils::Read<int>(finaladdr);
}

void Station::settechlevel(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x160, 0x1C});
    MemoryUtils::Write<int>(finaladdr, value);
}

bool Station::isvoid(void)
{
    if (getid() == -1)
        return true;
    return false;
}