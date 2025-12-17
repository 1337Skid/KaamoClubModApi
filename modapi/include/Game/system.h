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
        static inline uintptr_t system = 0;
    public:
        static void init(void);
        static int getsystemid(void);
        static void setsystemid(int value);
        static int getsystemrisklevel(void);
        static void setsystemrisklevel(int value);
        static int getsystemfaction(void);
        static void setsystemfaction(int value);
        static int getsystemjumpgatestationid(void);
        static void setsystemjumpgatestationid(int value);
        static int getsystemmapcoordinatex(void);
        static void setsystemmapcoordinatex(int value);
        static int getsystemmapcoordinatey(void);
        static void setsystemmapcoordinatey(int value);
        static int getsystemmapcoordinatez(void);
        static void setsystemmapcoordinatez(int value);
};

#endif