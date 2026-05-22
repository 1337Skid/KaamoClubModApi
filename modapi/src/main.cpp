#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include "patches.h"
#include "hooks.h"
#include <Game/system.h>
#include <Game/level.h>
#include <chrono>

DWORD WINAPI MainThread(LPVOID lpParam) {
    // TODO: CLEAN THE GOD DAMN HEADERS!!!!!!!!!!!!!
    LuaManager *luamanager = new LuaManager();
    FILE* dummyfile;
    freopen_s(&dummyfile, "CONOUT$", "w", stdout);
    freopen_s(&dummyfile, "CONOUT$", "w", stderr);
    freopen_s(&dummyfile, "CONIN$", "r", stdin);
    
    std::cout << "[+] KaamoClubModAPI Loaded! | Version: 1.0.2" << std::endl;    
    luamanager->init();
    luamanager->bind_api();
    EventManager::lua_manager = luamanager;
    ModApiUtils::load_mods(luamanager);
    EventManager::earlyinit_event();
    Hooks::init();
    Player::init(luamanager->getluastate());
    System::init();
    Station::init();
    Mission::init();
    Asset::init();
    //Item::init();
    //Ship::init();
    Level::init(luamanager->getluastate());
    auto last_tick = std::chrono::steady_clock::now();
    while (1) {
        auto current_tick = std::chrono::steady_clock::now();
        std::chrono::duration<float> ticks = current_tick - last_tick;
        float dt = ticks.count();
        last_tick = current_tick;
        luamanager->update(dt);
        EventManager::trigger_events();
        Sleep(10); // cpu won't fry with this one :fire:
    }
    if (dummyfile)
        fclose(dummyfile);
    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);        
        HANDLE hThread = CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
        if (hThread)
            CloseHandle(hThread);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}