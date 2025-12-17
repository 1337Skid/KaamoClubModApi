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
    station = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C;
}

std::string Station::getstationname()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(station, {0x160, 0x0, 0x0});
    return MemoryUtils::ReadWideString(finaladdr);
}

void Station::setstationname(const std::string value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(station, {0x160, 0x0, 0x0});
    MemoryUtils::WriteWideString(finaladdr, value);
}

int Station::getstationtechlevel()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(station, {0x160, 0x1C});
    return MemoryUtils::Read<int>(finaladdr);
}

void Station::setstationtechlevel(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(station, {0x160, 0x1C});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Station::getstationid()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(station, {0x160, 0x8});
    return MemoryUtils::Read<int>(finaladdr);
}

void Station::setstationid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(station, {0x160, 0x8});
    MemoryUtils::Write<int>(finaladdr, value);
}