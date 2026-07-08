#include "memoryutils.h"
#include "abyssengine.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include "offset.h"

System::System() : _ptr(nullptr) {}
System::System(SingleSystem *ptr) : _ptr(ptr) {};
System::~System() {};

void System::init()
{
    globals_status = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
}

SingleSystem *System::getstruct() const
{
    if (_ptr != nullptr)
        return _ptr;
    if (globals_status && *globals_status)
        return (*globals_status)->m_pSystemInfo;
    return nullptr;
}

int System::getid()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->id;
}

void System::setid(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->id = value;
}

int System::getrisklevel()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->risk;
}

void System::setrisklevel(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->risk = value;
}

int System::getfaction()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->faction;
}

void System::setfaction(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->faction = value;
}

int System::getjumpgatestationid()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->jumpgate_station_id;
}

void System::setjumpgatestationid(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->jumpgate_station_id = value;
}

int System::getmapcoordinatex()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->pos.x;
}

void System::setmapcoordinatex(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->pos.x = value;
}

int System::getmapcoordinatey()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->pos.y;
}

void System::setmapcoordinatey(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->pos.y = value;
}

int System::getmapcoordinatez()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->pos.z;
}

void System::setmapcoordinatez(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->pos.z = value;
}

int System::gettextureid()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return 0;
    return s->texture_id;
}

void System::settextureid(int value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    s->texture_id = value;
}

std::string System::getname()
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return "";
    uintptr_t strptr = reinterpret_cast<uintptr_t>(s->name.text);
    return MemoryUtils::ReadWideString(strptr);
}

void System::setname(std::string value)
{
    SingleSystem *s = getstruct();
    if (s == nullptr)
        return;
    uintptr_t strptr = reinterpret_cast<uintptr_t>(s->name.text);
    MemoryUtils::WriteWideString(strptr, value);
}

int System::create(const std::string& str, int x, int y, int z, int faction, int risk, int textureid, int linkedsystemid)
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
    s.jumpgate_station_id = 0;
    s.starts_unlocked = true;
    if (linkedsystemid != -1) {
        s.linked_system_ids = AbyssEngine::newarray<uint32_t>(1);
        s.linked_system_ids->data[0] = linkedsystemid;
    } else {
        s.linked_system_ids = nullptr;
    }
    s.station_ids = nullptr;
    s.faction = faction;
    s.risk = risk;
    s.texture_id = textureid;
    created_systems.push_back(s);
    return 26 + created_systems.size();
}

bool System::isvisible(int systemid)
{
    if ((*globals_status)->m_pSystemVisibilities == nullptr)
        return false;
    return reinterpret_cast<uint8_t*>((*globals_status)->m_pSystemVisibilities->data)[systemid] == 1;
}

void System::setvisible(int systemid, bool visible)
{
    if ((*globals_status)->m_pSystemVisibilities == nullptr)
        return;
    uint8_t* data = reinterpret_cast<uint8_t*>((*globals_status)->m_pSystemVisibilities->data);    
    data[systemid] = static_cast<uint8_t>(visible);
}

SingleSystem *System::getsystembyid(int id)
{
    auto *galaxy = *reinterpret_cast<Galaxy**>(Offset::GLOBALS_GALAXY);
    if (!galaxy || !galaxy->systems)
        return nullptr;
    auto* systems = galaxy->systems;
    if (id >= 0 && id < static_cast<int>(systems->size))
        return systems->data[id];
    return nullptr;
}