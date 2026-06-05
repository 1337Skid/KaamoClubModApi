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
#include <DbgHelp.h>
#include <chrono>
#include <sstream>
#include <fstream>
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
        static std::string gettimestamp(void);
        static std::string getexception_name(DWORD code);
    public:
        static void load_mods(LuaManager *luamanager);
        static std::string w2s(const std::wstring& wstr);
        static std::wstring s2w(const std::string& str);
        static LONG WINAPI crashhandler(EXCEPTION_POINTERS *ep);
};
#endif