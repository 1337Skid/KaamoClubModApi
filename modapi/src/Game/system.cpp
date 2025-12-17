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
    system = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C;
}

int System::getsystemid()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x14});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x14});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getsystemrisklevel()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x18});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemrisklevel(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x18});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getsystemfaction(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x1C});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemfaction(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x1C});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getsystemjumpgatestationid(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x2C});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemjumpgatestationid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x2C});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getsystemmapcoordinatex(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x20});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemmapcoordinatex(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x20});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getsystemmapcoordinatey(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x24});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemmapcoordinatey(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x24});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getsystemmapcoordinatez(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x28});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setsystemmapcoordinatez(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(system, {0x168, 0x28});
    MemoryUtils::Write<int>(finaladdr, value);
}