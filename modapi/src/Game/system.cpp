#include "memoryutils.h"
#include "abyssengine.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <thread>
#include <chrono>

void System::init()
{
    auto start = std::chrono::high_resolution_clock::now();
    uintptr_t base = MemoryUtils::GetModuleBase("GoF2.exe");
    
    while (globals_status == nullptr) {
        globals_status = *reinterpret_cast<Globals_status**>(base + 0x20AD6C); // Globals::status
        if (globals_status == nullptr)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("[+] System Initialization took: %lld ms\n", duration);
}

int System::getid()
{
    return globals_status->m_pSystemInfo->id;
}

void System::setid(int value)
{
    globals_status->m_pSystemInfo->id = value;
}

int System::getrisklevel()
{
    return globals_status->m_pSystemInfo->risk;
}

void System::setrisklevel(int value)
{
    globals_status->m_pSystemInfo->risk = value;
}

int System::getfaction(void)
{
    return globals_status->m_pSystemInfo->faction;
}

void System::setfaction(int value)
{
    globals_status->m_pSystemInfo->faction = value;
}

int System::getjumpgatestationid(void)
{
    return globals_status->m_pSystemInfo->jumpgate_station_id;
}

void System::setjumpgatestationid(int value)
{
    globals_status->m_pSystemInfo->jumpgate_station_id = value;
}

int System::getmapcoordinatex(void)
{
    return globals_status->m_pSystemInfo->pos.x;
}

void System::setmapcoordinatex(int value)
{
    globals_status->m_pSystemInfo->pos.x = value;
}

int System::getmapcoordinatey(void)
{
    return globals_status->m_pSystemInfo->pos.y;
}

void System::setmapcoordinatey(int value)
{
    globals_status->m_pSystemInfo->pos.y = value;
}

int System::getmapcoordinatez(void)
{
    return globals_status->m_pSystemInfo->pos.z;
}

void System::setmapcoordinatez(int value)
{
    globals_status->m_pSystemInfo->pos.z = value;
}

std::string System::getname()
{
    uintptr_t strptr = reinterpret_cast<uintptr_t>(globals_status->m_pSystemInfo->name.text);

    return MemoryUtils::ReadWideString(strptr);
}

void System::setname(std::string value)
{
    uintptr_t strptr = reinterpret_cast<uintptr_t>(globals_status->m_pSystemInfo->name.text);

    MemoryUtils::WriteWideString(strptr, value);
}

int System::create(const std::string& str, int x, int y, int z, int faction, int risk, int textureid)
{
    if (EventManager::isearlyinit_finished) {
        std::cout << "[-] Failed to call system:Create(), you can only call it in the EarlyInit event" << std::endl;
        return -1;
    }

    SingleSystem s;

    // utf8 to utf16 bruh I hate this game (sol2 can't pass a wchar directly -_-)
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring out(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, out.data(), len);

    s.name = AbyssEngine::newstring(out.c_str());
    s.pos  = { x, y, z };
    s.id = 0;
    s.jumpgate_station_id = 30;
    s.starts_unlocked = true;
    s.linked_system_ids = nullptr;
    s.station_ids = nullptr;
    s.faction = faction;
    s.risk = risk;
    s.texture_id = textureid;

    created_systems.push_back(s);
    return 26 + created_systems.size();
}

bool System::isvisible(int systemid)
{
    return reinterpret_cast<uint8_t*>(globals_status->m_pSystemVisibilities->data)[systemid] == 1;
}

void System::setvisible(int systemid, bool visible)
{
    uint8_t* data = reinterpret_cast<uint8_t*>(globals_status->m_pSystemVisibilities->data);    
    data[systemid] = (uint8_t)visible;
}