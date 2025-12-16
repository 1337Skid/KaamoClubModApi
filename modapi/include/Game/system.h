#ifndef SYSTEM_H
#define SYSTEM_H
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

class System {
    private:
        static inline uintptr_t systemid = 0;
    public:
        static void init(void);
        static int getsystemid(void);
        static void setsystemid(int value);
};

#endif