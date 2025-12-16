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

void System::init()
{
    systemid = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C;
}

int System::getsystemid()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(systemid, {0x168, 0x14});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(systemid, {0x168, 0x14});
    MemoryUtils::Write<int>(finaladdr, value);
}