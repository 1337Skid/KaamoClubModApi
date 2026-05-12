#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <codecvt>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include "abyssengine.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

DWORD ModApiUtils::getmainthreadid() 
{
    DWORD pid = GetCurrentProcessId();
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te32;
    DWORD mainthreadid = 0;
    unsigned long long oldestTime = (unsigned long long)-1;

    if (hSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    te32.dwSize = sizeof(THREADENTRY32);
    if (Thread32First(hSnapshot, &te32)) {
        do {
            if (te32.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, te32.th32ThreadID);
                if (hThread) {
                    FILETIME ftCreation, ftExit, ftKernel, ftUser;
                    if (GetThreadTimes(hThread, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
                        unsigned long long time = ((unsigned long long)ftCreation.dwHighDateTime << 32) | ftCreation.dwLowDateTime;
                        if (time < oldestTime) {
                            oldestTime = time;
                            mainthreadid = te32.th32ThreadID;
                        }
                    }
                    CloseHandle(hThread);
                }
            }
        } while (Thread32Next(hSnapshot, &te32));
    }
    CloseHandle(hSnapshot);
    return mainthreadid;
}

void ModApiUtils::suspendgame(bool suspend)
{
    DWORD mainthreadid = getmainthreadid();

    if (mainthreadid == 0)
        return;

    HANDLE hMainThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, mainthreadid);

    if (hMainThread) {
        if (suspend)
            SuspendThread(hMainThread);
        else 
            while (ResumeThread(hMainThread) > 1);
        CloseHandle(hMainThread);
    } else {
        std::cout << "[-] Couldn't open main game thread??? black magic????" << std::endl;
    }
}

void ModApiUtils::load_mods(LuaManager *luamanager)
{    
    if (!std::filesystem::exists("mods") || !std::filesystem::is_directory("mods"))
        std::filesystem::create_directory("mods");
    for (const auto& entry : std::filesystem::directory_iterator("mods")) {
        if (entry.is_directory()) {
            std::string mod_path = entry.path().string();
            std::string init_lua = mod_path + "/init.lua";
            if (std::filesystem::exists(init_lua)) {
                std::cout << "[+] Loaded mod: " << entry.path().filename().string() << std::endl;
                luamanager->execute_script(init_lua);
            }
        }
    }    
}

// credits https://gist.github.com/danzek/d6a0e4a48a5439e7f808ed1497f6268e
std::string ModApiUtils::w2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}