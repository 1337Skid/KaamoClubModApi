#ifndef STATION_H
#define STATION_H
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

class Station {
    private:
        static inline uintptr_t globals_status = 0;
    public:
        static void init(void);
        static int getid(void);
        static void setid(int value);
        static std::string getname(void);
        static void setname(const std::string value);
        static int gettechlevel(void);
        static void settechlevel(int value);
        
        static bool isvoid(void);
};
#endif