#ifndef ASSET_H
#define ASSET_H
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

class Asset {
    private:
        static inline uintptr_t assetfilepath = 0;
    public:
        static void init(void);
        static std::string getassetfilepath(unsigned int offset);
        static void setassetfilepath(unsigned int offset, const std::string value);
};
#endif