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
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int Station::getid()
{
    if (globals_status->m_pStationInfo == nullptr)
        return 0;
    return globals_status->m_pStationInfo->id;
}

void Station::setid(int value)
{
    if (globals_status->m_pStationInfo == nullptr)
        return;
    globals_status->m_pStationInfo->id = value;
}

std::string Station::getname()
{
    if (globals_status->m_pStationInfo->name.text == nullptr)
        return "";
    return MemoryUtils::ReadWideString(reinterpret_cast<uintptr_t>(globals_status->m_pStationInfo->name.text));
}

void Station::setname(const std::string value)
{
    if (globals_status->m_pStationInfo->name.text == nullptr)
        return;
    MemoryUtils::WriteWideString(reinterpret_cast<uintptr_t>(globals_status->m_pStationInfo->name.text), value);
}

int Station::gettechlevel()
{
    if (globals_status->m_pStationInfo == nullptr)
        return 0;
    return globals_status->m_pStationInfo->techlevel;
}

void Station::settechlevel(int value)
{
    if (globals_status->m_pStationInfo == nullptr)
        return;
    globals_status->m_pStationInfo->techlevel = value;
}

bool Station::isvoid(void)
{
    if (getid() == -1)
        return true;
    return false;
}

int Station::gethangaritemscount()
{
    if (globals_status->m_pStationInfo->m_pItemsInHangar == nullptr)
        return 0;
    return globals_status->m_pStationInfo->m_pItemsInHangar->size;
}

bool Station::hasiteminhangar(int id)
{
    Sleep(1); // TODO: hell no | sleep 1 bcz the eventmanager is goofy :skull:
    if (globals_status->m_pStationInfo->m_pItemsInHangar == nullptr)
        return false;
    AEArray<SingleItem*>* array = (AEArray<SingleItem*>*)globals_status->m_pStationInfo->m_pItemsInHangar;
    for (uint32_t i = 0; i < array->size; i++) {
        if (array->data[i] != nullptr && array->data[i]->m_nID == id) {
            return true;
        }
    }
    return false;
}

void Station::removehangaritem(int id)
{
    Sleep(1); // TODO: hell no | sleep 1 bcz the eventmanager is goofy :skull:
    if (globals_status->m_pStationInfo->m_pItemsInHangar == nullptr)
        return;
    AEArray<SingleItem*>* oldarray = (AEArray<SingleItem*>*)globals_status->m_pStationInfo->m_pItemsInHangar;
    int finalid = -1;
    for (uint32_t i = 0; i < oldarray->size; i++) {
        if (oldarray->data[i] != nullptr && oldarray->data[i]->m_nID == id) {
            finalid = static_cast<int>(i);
            break; 
        }
    }
    if (finalid == -1) {
        std::cout << "[-] Cannot find item pointer with id: " << id << std::endl;
        return;
    }
    uint32_t newsize = oldarray->size - 1;
    if (newsize == 0) {
        AbyssEngine::memory_free(oldarray->data);
        AbyssEngine::memory_free(oldarray);
        globals_status->m_pStationInfo->m_pItemsInHangar = nullptr;
        return;
    }
    AEArray<SingleItem*>* newarray = AbyssEngine::newarray<SingleItem*>(newsize);
    if (!newarray)
        return;
    uint32_t currentNewIdx = 0;
    for (uint32_t i = 0; i < oldarray->size; i++) {
        if (i == static_cast<uint32_t>(finalid))
            continue; 
        newarray->data[currentNewIdx] = oldarray->data[i];
        currentNewIdx++;
    }
    AbyssEngine::memory_free(oldarray->data);
    AbyssEngine::memory_free(oldarray);
    globals_status->m_pStationInfo->m_pItemsInHangar = (AEArray<SingleItem>*)newarray;    
}

void Station::sethangaritemscount(int value)
{
    if (globals_status->m_pStationInfo->m_pItemsInHangar == nullptr)
        return;
    globals_status->m_pStationInfo->m_pItemsInHangar->size = value;
    globals_status->m_pStationInfo->m_pItemsInHangar->size2 = value;
}

int Station::gethangarshipscount()
{
    if (globals_status->m_pStationInfo->m_pShipsInHangar == nullptr)
        return 0;
    return globals_status->m_pStationInfo->m_pShipsInHangar->size;
}

void Station::sethangarshipscount(int value)
{
    if (globals_status->m_pStationInfo->m_pShipsInHangar == nullptr)
        return;
    globals_status->m_pStationInfo->m_pShipsInHangar->size = value;
    globals_status->m_pStationInfo->m_pShipsInHangar->size2 = value;
}

void Station::sethangarshipinfo(int id, sol::table shipinfo)
{
    if (globals_status->m_pStationInfo->m_pShipsInHangar == nullptr)
        return;
    unsigned int offset = (id == 0) ? 0 : (1 << (id + 1));
    auto* shipsarray = globals_status->m_pStationInfo->m_pShipsInHangar;
    uint8_t* arraydata = reinterpret_cast<uint8_t*>(shipsarray->data);
    ShipInfo** ship_ptr = reinterpret_cast<ShipInfo**>(arraydata + offset);
    
    if (*ship_ptr) {
        (*ship_ptr)->m_nID = shipinfo["id"].get_or<int>((*ship_ptr)->m_nID);
        (*ship_ptr)->m_nPrice = shipinfo["price"].get_or<int>((*ship_ptr)->m_nPrice);
    }
}

int Station::getagentscount()
{
    if (globals_status->m_pStationInfo->m_pAgents == nullptr) //TODO: Sometimes agents is null if it's in systemchanged event??
        return 0;
    return globals_status->m_pStationInfo->m_pAgents->size;
}

void Station::setagentscount(int value)
{
    if (globals_status->m_pStationInfo->m_pAgents == nullptr)
        return;
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
    s.m_pShipsInHangar = 0;
    s.m_pItemsInHangar = 0;
    s.m_pAgents = 0;

    created_stations.push_back(s);
    return 108 + created_stations.size();
}

std::string Station::getagentname(int id)
{
    if (globals_status->m_pStationInfo->m_pAgents == nullptr)
        return "";
    SingleAgent** agents_list = reinterpret_cast<SingleAgent**>(globals_status->m_pStationInfo->m_pAgents->data);
    SingleAgent* agent = agents_list[id];
    return MemoryUtils::ReadWideString(reinterpret_cast<uintptr_t>(agent->m_sName.text));
}

int Station::getagentfaction(int id)
{
    if (globals_status->m_pStationInfo->m_pAgents == nullptr)
        return 0;
    SingleAgent** agents_list = reinterpret_cast<SingleAgent**>(globals_status->m_pStationInfo->m_pAgents->data);
    SingleAgent* agent = agents_list[id];
    return agent->m_nFactionType;
}

void Station::createagent(const std::string& name, int factiontype, int terranwoman, int hair, int eyes, int mouth, int armor, sol::table agentinfo)
{
    if (globals_status->m_pStationInfo == nullptr)
        return;
    if (!agentinfo) {
        std::cout << "[-] Cannot create agent '" << name << "' because its missing the agentinfo table !" << std::endl;
        return;
    }
    Sleep(1); // TODO: hell no | sleep 1 bcz the eventmanager is goofy :skull:
    SingleStation* station = globals_status->m_pStationInfo;
    AEArray<SingleAgent*>* oldarray = reinterpret_cast<AEArray<SingleAgent*>*>(station->m_pAgents);
    if (oldarray != nullptr && oldarray->size2 == 7) {
        std::cout << "[-] Cannot create agent '" << name << "' because the space lounge has reached the limit (7) you might need to check agents count to avoid duplicates?" << std::endl;
        return;
    }
    uint32_t oldsize = (oldarray != nullptr) ? oldarray->size : 0;
    AEArray<SingleAgent*>* newarray = AbyssEngine::newarray<SingleAgent*>(oldsize + 1);

    if (!newarray) return;
    if (oldarray != nullptr && oldarray->data != nullptr) {
        for (uint32_t i = 0; i < oldsize; ++i)
            newarray->data[i] = oldarray->data[i];
    }
    SingleAgent* pNewAgent = reinterpret_cast<SingleAgent*>(AbyssEngine::memory_allocate(sizeof(SingleAgent)));
    if (pNewAgent) {
        memset(pNewAgent, 0, sizeof(SingleAgent));
        int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, NULL, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, buf, len);
        pNewAgent->m_sName = AbyssEngine::newstring(buf);
        delete[] buf;
        pNewAgent->field_08 = 0;
        pNewAgent->field_0C = 0;
        pNewAgent->field_10 = 0;
        pNewAgent->field_14 = 0;         
        pNewAgent->field_18 = 0;
        pNewAgent->field_1C = 0;
        if (agentinfo["talking_type"] == 1 || agentinfo["talking_type"] == 2 && agentinfo["talking_string"])
            pNewAgent->m_nTalkingStringLangId1 = agentinfo["talking_string"];
        else if(agentinfo["talking_type"] == 2)
            pNewAgent->m_nTalkingStringLangId1 = 750; // fallback
        else
            pNewAgent->m_nTalkingStringLangId1 = 803; // fallback
        if (agentinfo["talking_type"] == 2 && agentinfo["talking_string_2"])
            pNewAgent->m_nTalkingStringLangId2 = agentinfo["talking_string_2"];
        else if(agentinfo["talking_type"] == 2)
            pNewAgent->m_nTalkingStringLangId2 = 753; // fallback
        else
            pNewAgent->m_nTalkingStringLangId2 = -1;
        if (agentinfo["talking_type"] == 2 && agentinfo["item_selling"])
            pNewAgent->m_nItemSellingId = agentinfo["item_selling"];
        else
            pNewAgent->m_nItemSellingId = 0;
        if (agentinfo["talking_type"] == 2 && agentinfo["item_count"])
            pNewAgent->m_nItemCount = agentinfo["item_count"];
        else
            pNewAgent->m_nItemCount = 0;
        if (agentinfo["talking_type"] == 2 && agentinfo["item_price"])
            pNewAgent->m_nItemPrice = agentinfo["item_price"];
        else if(agentinfo["talking_type"] == 2)
            pNewAgent->m_nItemPrice = 0;
        else
            pNewAgent->m_nItemPrice = -1;
        pNewAgent->m_nItemStringLangId = -1;
        pNewAgent->field_38 = 0;
        pNewAgent->field_3C = 0;
        pNewAgent->m_nFactionType = factiontype;
        if (factiontype == 0 && terranwoman == 1)
            pNewAgent->m_nIsTerranWoman = 1;
        else
            pNewAgent->m_nIsTerranWoman = 0;
        if (agentinfo["auto_talking_string"])
            pNewAgent->m_nGreetings = 0; // yes 0, this is intended
        else
            pNewAgent->m_nGreetings = 1;
        pNewAgent->field_4C = 1;
        if (agentinfo["talking_type"] == 1 || agentinfo["talking_type"] == 2)
            pNewAgent->m_nTalkingType = agentinfo["talking_type"]; // 2 = seller
        else
            pNewAgent->m_nTalkingType = 1;
        pNewAgent->m_nAlliesPrice = 0;
        pNewAgent->field_58 = -1;
        pNewAgent->field_5C = -1;
        pNewAgent->field_60 = nullptr;
        pNewAgent->field_64 = 0;
        pNewAgent->field_68 = nullptr;
        pNewAgent->field_6C = 0;
        pNewAgent->field_70 = nullptr;
        pNewAgent->m_pAgentTextureFace = reinterpret_cast<AgentTextureFaceInfo*>(AbyssEngine::memory_allocate(sizeof(AgentTextureFaceInfo)));
        pNewAgent->m_pAgentTextureFace->faction_type = factiontype;
        pNewAgent->m_pAgentTextureFace->mouth = mouth;
        pNewAgent->m_pAgentTextureFace->eyes = eyes;
        pNewAgent->m_pAgentTextureFace->armor = armor;
        pNewAgent->m_pAgentTextureFace->hair = hair;
        pNewAgent->m_pAgentTextureFace->field_14 = 0;
        pNewAgent->m_pMissionInfo = nullptr;
        pNewAgent->m_pRecruitedAllies = nullptr;
        pNewAgent->field_80 = 0;
        newarray->data[oldsize] = pNewAgent;
    }
    station->m_pAgents = reinterpret_cast<AEArray<SingleAgent>*>(newarray);
}