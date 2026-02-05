#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <MinHook.h>
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
#include "patches.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/structs.h>

void Patches::patchstarmap(uint8_t new_limit)
{
    DWORD old;

    VirtualProtect((LPVOID)0x004CE771, 2, PAGE_EXECUTE_READWRITE, &old);
    *reinterpret_cast<uint8_t*>((0x004CE771 + 1)) = new_limit; // push 27 asm
    VirtualProtect((LPVOID)0x004CE771, 2, old, &old);
}

// This patch is no longer used, it was useful to add a new station into stations.bin but now we are making stations in the runtime
// I didn't delete it just incase it's useful for anyone
void Patches::patchloadstations(uint8_t new_limit)
{
    DWORD old;
    uintptr_t addrs[] = { 0x004089B9, 0x00408DFE };

    for (size_t i = 0; i < 2; i++) {
        uintptr_t addr = addrs[i];
        VirtualProtect((LPVOID)addr, 1, PAGE_EXECUTE_READWRITE, &old);
        *reinterpret_cast<uint8_t*>(addr) = new_limit; // 0x6E
        VirtualProtect((LPVOID)addr, 1, old, &old);
    }
}

void Patches::patchmissions(uint8_t new_value)
{
    DWORD old;
    uintptr_t addr = 0x0049E212;

    VirtualProtect((LPVOID)addr, 1, PAGE_EXECUTE_READWRITE, &old);
    *reinterpret_cast<uint8_t*>(addr) = new_value;
    VirtualProtect((LPVOID)addr, 1, old, &old);
}

void Patches::patchvalkyrie()
{
    DWORD old;
    uintptr_t addrs[] = { 0x0049E1FD, 0x0049E205 };

    for (size_t i = 0; i < 2; i++) {
        uintptr_t addr = addrs[i];
        VirtualProtect((LPVOID)addr, 1, PAGE_EXECUTE_READWRITE, &old);
        *reinterpret_cast<uint8_t*>(addr) = 0x74; // jz
        VirtualProtect((LPVOID)addr, 1, old, &old);
    }
    MemoryUtils::Write<int>(MemoryUtils::GetModuleBase("GOF2.exe") + 0x20AD4D, 1); // is valkyrie
    MemoryUtils::Write<int>(MemoryUtils::GetModuleBase("GOF2.exe") + 0x20AD4F, 1); // is valkyrie
}