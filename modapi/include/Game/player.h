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
        static inline uintptr_t globals_status = 0;
        static inline uintptr_t globals_appmanager = 0;
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
        static int getmaxshiphealth(void);
        static void setmaxshiphealth(int value);
        static int getenemieskilled(void);
        static void setenemieskilled(int value);
        static int getlevel(void);
        static void setlevel(int value);
        static int getvisitedstations(void);
        static void setvisitedstations(int value);
        static int getjumpgateusedcount(void);
        static void setjumpgateusedcount(int value);
        static int getcargotookcount(void);
        static void setcargotookcount(int value);

        static bool hasshiparmor(void);
        static bool isdocked(void);
};
#endif