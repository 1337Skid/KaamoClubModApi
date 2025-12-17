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
        static inline uintptr_t station = 0;
    public:
        static void init(void);
        static std::string getstationname(void);
        static void setstationname(const std::string value);
        static int getstationtechlevel(void);
        static void setstationtechlevel(int value);
        static int getstationid(void);
        static void setstationid(int value);
};
#endif