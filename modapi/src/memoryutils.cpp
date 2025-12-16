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

uintptr_t MemoryUtils::GetModuleBase(const char* modulename) {
    return (uintptr_t)GetModuleHandleA(modulename);
}

uintptr_t MemoryUtils::GetPointerAddress(uintptr_t startaddr, const std::vector<unsigned int>& offsets) {
    uintptr_t addr = startaddr;

    for (unsigned int i = 0; i < offsets.size(); ++i) {
        ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, &addr, sizeof(addr), NULL);
        if (addr == 0)
            return 0;
        addr += offsets[i];
    }
    return addr;
}