#include "memoryutils.h"
#include "abyssengine.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <thread>
#include <chrono>

void Station::init()
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
    printf("[+] Station Initialization took: %lld ms\n", duration);
}

int Station::getid()
{
    return globals_status->m_pStationInfo->id;
}

void Station::setid(int value)
{
    globals_status->m_pStationInfo->id = value;
}

std::string Station::getname()
{
    return MemoryUtils::ReadWideString(reinterpret_cast<uintptr_t>(globals_status->m_pStationInfo->name.text));
}

void Station::setname(const std::string value)
{
    MemoryUtils::WriteWideString(reinterpret_cast<uintptr_t>(globals_status->m_pStationInfo->name.text), value);
}

int Station::gettechlevel()
{
    return globals_status->m_pStationInfo->techlevel;
}

void Station::settechlevel(int value)
{
    globals_status->m_pStationInfo->techlevel = value;
}

bool Station::isvoid(void)
{
    if (getid() == -1)
        return true;
    return false;
}

int Station::getangaritemscount()
{
    if (globals_status->m_pStationInfo->m_pItemsInAngar == nullptr)
        return 0;
    return globals_status->m_pStationInfo->m_pItemsInAngar->size;
}

void Station::setangaritemscount(int value)
{
    globals_status->m_pStationInfo->m_pItemsInAngar->size = value;
    globals_status->m_pStationInfo->m_pItemsInAngar->size2 = value;
}

int Station::getangarshipscount()
{
    if (globals_status->m_pStationInfo->m_pAgents == nullptr)
        return 0;
    return globals_status->m_pStationInfo->m_pShipsInAngar->size;
}

void Station::setangarshipscount(int value)
{
    globals_status->m_pStationInfo->m_pShipsInAngar->size = value;
    globals_status->m_pStationInfo->m_pShipsInAngar->size2 = value;
}

void Station::setangarshipid(int id, int value)
{
    unsigned int offset = (id == 0) ? 0 : (1 << (id + 1));
    auto* shipsarray = globals_status->m_pStationInfo->m_pShipsInAngar;
    uint8_t* arraydata = reinterpret_cast<uint8_t*>(shipsarray->data);
    SingleItem** ship_ptr = reinterpret_cast<SingleItem**>(arraydata + offset);
    
    if (*ship_ptr)
        (*ship_ptr)->m_nID = value;
}

int Station::getagentscount()
{
    if (globals_status->m_pStationInfo->m_pAgents == nullptr)
        return 0;
    return globals_status->m_pStationInfo->m_pAgents->size;
}

void Station::setagentscount(int value)
{
    globals_status->m_pStationInfo->m_pAgents->size = value;
    globals_status->m_pStationInfo->m_pAgents->size2 = value;
}

int Station::create(const std::string& str, int techlevel, int textureid, int systemid)
{
    if (EventManager::isearlyinit_finished) {
        std::cout << "[-] Failed to call station:Create(), you can only call it in the EarlyInit event" << std::endl;
        return -1;
    }
    SingleStation s;
    
    memset(&s, 0, sizeof(SingleStation));

    // utf8 to utf16 bruh I hate this game (sol2 can't pass a wchar directly -_-)
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring out(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, out.data(), len);

    s.name = AbyssEngine::newstring(out.c_str());
    s.id = created_stations.empty() ? 109 : 109 + created_stations.size();
    s.systemid = systemid;
    s.techlevel = techlevel;
    s.textureid = textureid;
    s.unk0 = 0; s.unk1 = 0; s.unk2 = 0;
    s.m_pShipsInAngar = 0;
    s.m_pItemsInAngar = 0;
    s.m_pAgents = 0;

    created_stations.push_back(s);
    return 108 + created_stations.size();
}