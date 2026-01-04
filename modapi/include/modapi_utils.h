#ifndef MODAPIUTILS_H
#define MODAPIUTILS_H
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

class ModApiUtils {
    private:
        static DWORD getmainthreadid(void);
    public:
        static void suspendgame(bool suspend);
        static void load_mods(LuaManager *luamanager);
};
#endif