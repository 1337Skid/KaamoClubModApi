#ifndef PLAYER_H
#define PLAYER_H
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

class Player {
    private:
        static inline uintptr_t money = 0;
        static inline uintptr_t maxcargo = 0;
        static inline uintptr_t cargo = 0;
        static inline uintptr_t shiparmor = 0;
    public:
        static void init(void);
        static int getmoney(void);
        static void setmoney(int value);
        static int getmaxcargo(void);
        static void setmaxcargo(int value);
        static int getcargo(void);
        static void setcargo(int value);
        static int getshiparmor(void);
        static void setshiparmor(int value);

        static bool hasshiparmor(void);
};
#endif