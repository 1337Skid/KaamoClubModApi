#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include "patches.h"
#include "hooks.h"
#include <Game/system.h>

DWORD WINAPI MainThread(LPVOID lpParam) {
    // TODO: CLEAN THE GOD DAMN HEADERS!!!!!!!!!!!!!
    LuaManager *luamanager = new LuaManager();
    FILE* dummyfile;
    freopen_s(&dummyfile, "CONOUT$", "w", stdout);
    freopen_s(&dummyfile, "CONOUT$", "w", stderr);
    
    std::cout << "[+] KaamoClubModAPI Loaded! | Version: dev-alpha" << std::endl;
    Player::init();
    System::init();
    Station::init();
    Mission::init();
    Asset::init();
    luamanager->init();
    luamanager->bind_api();
    ModApiUtils::load_mods(luamanager);
    //System::create(L"test", 80, 30, 47);
    //System::create(L"test blabla", 80, 50, 50);
    EventManager::earlyinit_event();
    Hooks::init();
    while (true)
        EventManager::trigger_events();

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