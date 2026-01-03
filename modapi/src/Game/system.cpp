#include "memoryutils.h"
#include "abyssengine.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

void System::init()
{
    while (globals_status == 0) globals_status = *reinterpret_cast<Globals_status**>(MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C); // Globals::status
}

int System::getid()
{
    return globals_status->m_pCurrentSystem->id;
}

void System::setid(int value)
{
    globals_status->m_pCurrentSystem->id = value;
}

int System::getrisklevel()
{
    return globals_status->m_pCurrentSystem->risk;
}

void System::setrisklevel(int value)
{
    globals_status->m_pCurrentSystem->risk = value;
}

int System::getfaction(void)
{
    return globals_status->m_pCurrentSystem->faction;
}

void System::setfaction(int value)
{
    globals_status->m_pCurrentSystem->faction = value;
}

int System::getjumpgatestationid(void)
{
    return globals_status->m_pCurrentSystem->jumpgate_station_id;
}

void System::setjumpgatestationid(int value)
{
    globals_status->m_pCurrentSystem->jumpgate_station_id = value;
}

int System::getmapcoordinatex(void)
{
    return globals_status->m_pCurrentSystem->pos.x;
}

void System::setmapcoordinatex(int value)
{
    globals_status->m_pCurrentSystem->pos.x = value;
}

int System::getmapcoordinatey(void)
{
    return globals_status->m_pCurrentSystem->pos.y;
}

void System::setmapcoordinatey(int value)
{
    globals_status->m_pCurrentSystem->pos.y = value;
}

int System::getmapcoordinatez(void)
{
    return globals_status->m_pCurrentSystem->pos.z;
}

void System::setmapcoordinatez(int value)
{
    globals_status->m_pCurrentSystem->pos.z = value;
}

std::string System::getname()
{
    uintptr_t strptr = reinterpret_cast<uintptr_t>(globals_status->m_pCurrentSystem->name.text);

    return MemoryUtils::ReadWideString(strptr);
}

void System::setname(std::string value)
{
    uintptr_t strptr = reinterpret_cast<uintptr_t>(globals_status->m_pCurrentSystem->name.text);

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
    return globals_status->m_pSystemVisibilities->data[systemid] == 1;
}

void System::setvisible(int systemid, bool visible)
{
    uint8_t* data = reinterpret_cast<uint8_t*>(globals_status->m_pSystemVisibilities->data);    
    data[systemid] = (uint8_t)visible;
}