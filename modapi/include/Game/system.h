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
        static inline uintptr_t global_status = 0;
    public:
        static void init(void);
        static int getid(void);
        static void setid(int value);
        static int getrisklevel(void);
        static void setrisklevel(int value);
        static int getfaction(void);
        static void setfaction(int value);
        static int getjumpgatestationid(void);
        static void setjumpgatestationid(int value);
        static int getmapcoordinatex(void);
        static void setmapcoordinatex(int value);
        static int getmapcoordinatey(void);
        static void setmapcoordinatey(int value);
        static int getmapcoordinatez(void);
        static void setmapcoordinatez(int value);
        static std::string getname(void);
        static void setname(std::string value);
};

#endif