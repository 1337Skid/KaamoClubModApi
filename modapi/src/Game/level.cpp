#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/level.h>
#include <thread>
#include <chrono>
#include "patches.h"

void Level::init()
{
    auto start = std::chrono::high_resolution_clock::now();
    uintptr_t base = MemoryUtils::GetModuleBase("GoF2.exe");

    while (globals_appmanager == nullptr) {
        globals_appmanager = *reinterpret_cast<Globals_appManager**>(base + 0x20AEFC); // Globals::appManager
        if (globals_appmanager == nullptr)
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// TODO: do imageinfo (do stuff in hooks.cpp Hooks::imagefactory_drawchar_hook)
void Level::createradiomessage(const std::string& name, const std::string& content, sol::table imageinfo)
{
    uintptr_t address = 0x479678;
    MGame* mgame = reinterpret_cast<MGame*>(globals_appmanager->m_pCurrentModule);

    if (!mgame) {
        std::cout << "[-] Cannot call Level::createradiomessage because MGame isn't init!" << std::endl;
        return;
    }
    int* level = mgame->m_pLevel;
    if (!level) {
        std::cout << "[-] Cannot call Level::createradiomessage because Level isn't init!" << std::endl;
        return;
    }
    int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
    std::wstring namews(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, namews.data(), len);
    len = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, nullptr, 0);
    std::wstring contentws(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, contentws.data(), len);
    CustomRadioMsg crm;
    crm.name = namews;
    crm.content = contentws;
    created_radiomessages.push_back(crm);
    int a3 = 0;
    int a2 = 8;
    __asm {
        push a3
        push level
        mov ecx, a2
        call address
    }
}