#include "memoryutils.h"
#include "abyssengine.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

void System::init()
{
    global_status = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C; // Gloabals::status
}

int System::getid()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x14});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x14});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getrisklevel()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x18});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setrisklevel(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x18});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getfaction(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x1C});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setfaction(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x1C});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getjumpgatestationid(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x2C});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setjumpgatestationid(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x2C});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getmapcoordinatex(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x20});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setmapcoordinatex(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x20});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getmapcoordinatey(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x24});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setmapcoordinatey(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x24});
    MemoryUtils::Write<int>(finaladdr, value);
}

int System::getmapcoordinatez(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x28});
    return MemoryUtils::Read<int>(finaladdr);
}

void System::setmapcoordinatez(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0x28});
    MemoryUtils::Write<int>(finaladdr, value);
}

std::string System::getname()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0xC});
    uintptr_t strptr = MemoryUtils::Read<uintptr_t>(finaladdr);

    return MemoryUtils::ReadWideString(strptr);
}

void System::setname(std::string value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(global_status, {0x168, 0xC});
    uintptr_t strptr = MemoryUtils::Read<uintptr_t>(finaladdr);

    MemoryUtils::WriteWideString(strptr, value);
}

void System::create(const std::string& str, int x, int y, int z)
{
    if (EventManager::isearlyinit_finished) {
        std::cout << "[-] Failed to call system:Create(), you can only call it in the EarlyInit event" << std::endl;
        return;
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

    created_systems.push_back(s);
}
