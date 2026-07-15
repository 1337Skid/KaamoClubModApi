#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include "offset.h"

void Player::init(lua_State* lua_state)
{
    globals_status = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
    globals_appmanager = reinterpret_cast<Globals_appManager**>(Offset::GLOBALS_APPMANAGER);
    lstate = lua_state;
}

int Player::getmoney()
{
    return (*globals_status)->m_nMoney;
}

void Player::setmoney(int value)
{
    (*globals_status)->m_nMoney = value;
}

void Player::togglecloaking()
{
    uintptr_t address = Offset::PLAYEREGO_TOGGLECLOAKING;
    MGame* mgame = reinterpret_cast<MGame*>((*globals_appmanager)->m_pCurrentModule);
    PlayerEgo* playerego = mgame->m_pPlayerEgo;

    if (!playerego) {
        std::cout << "[-] Cannot call Player::togglecloaking because MGame isn't init!" << std::endl;
        return;
    }
    __asm {
        mov ebx, playerego
        call address
    }
}

void Player::setposition(float x, float y, float z)
{
    uintptr_t address = Offset::PLAYEREGO_SETPOSITION;
    MGame* mgame = reinterpret_cast<MGame*>((*globals_appmanager)->m_pCurrentModule);
    PlayerEgo* playerego = mgame->m_pPlayerEgo;

    if (!playerego) {
        std::cout << "[-] Cannot call Player::setposition because MGame isn't init!" << std::endl;
        return;
    }
    __asm {
        push z
        push y
        push x
        mov eax, playerego
        call address
    }
}

sol::table Player::getshipinfo()
{
    sol::state_view lua(lstate);
    sol::table shipinfo = lua.create_table();

    if (globals_status == nullptr || *globals_status == nullptr)
        return shipinfo;
    if ((*globals_status)->m_pShipInfo == nullptr)
        return shipinfo;
    shipinfo["id"] = (*globals_status)->m_pShipInfo->m_nID;
    shipinfo["maxhealth"] = (*globals_status)->m_pShipInfo->m_nMaxHealth;
    shipinfo["baseprice"] = (*globals_status)->m_pShipInfo->m_nBasePrice;
    shipinfo["maxcargo"] = (*globals_status)->m_pShipInfo->m_nMaxCapacity;
    shipinfo["cargo"] = (*globals_status)->m_pShipInfo->m_nCurrentCapacity;
    shipinfo["price"] = (*globals_status)->m_pShipInfo->m_nPrice;
    shipinfo["controlmultiplier"] = (*globals_status)->m_pShipInfo->m_fControlMultiplier;
    shipinfo["armor"]  = (*globals_status)->m_pShipInfo->m_nArmor;
    shipinfo["bonuscargo"] = (*globals_status)->m_pShipInfo->m_nBonusCapacity;
    shipinfo["power"] = (*globals_status)->m_pShipInfo->m_fPower;
    return shipinfo;
}

void Player::setshipinfo(sol::table shipinfo)
{
    if (!shipinfo) {
        std::cout << "[-] Shipinfo can't be null" << std::endl;
        return;
    }
    (*globals_status)->m_pShipInfo->m_nID = shipinfo["id"].get_or<int>((*globals_status)->m_pShipInfo->m_nID);
    (*globals_status)->m_pShipInfo->m_nMaxHealth = shipinfo["maxhealth"].get_or<int>((*globals_status)->m_pShipInfo->m_nMaxHealth);
    (*globals_status)->m_pShipInfo->m_nBasePrice = shipinfo["baseprice"].get_or<int>((*globals_status)->m_pShipInfo->m_nBasePrice);
    (*globals_status)->m_pShipInfo->m_nMaxCapacity = shipinfo["maxcargo"].get_or<int>((*globals_status)->m_pShipInfo->m_nMaxCapacity);
    (*globals_status)->m_pShipInfo->m_nCurrentCapacity = shipinfo["cargo"].get_or<int>((*globals_status)->m_pShipInfo->m_nCurrentCapacity);
    (*globals_status)->m_pShipInfo->m_nPrice = shipinfo["price"].get_or<int>((*globals_status)->m_pShipInfo->m_nPrice);
    (*globals_status)->m_pShipInfo->m_fControlMultiplier = shipinfo["controlmultiplier"].get_or<int>((*globals_status)->m_pShipInfo->m_fControlMultiplier);
    (*globals_status)->m_pShipInfo->m_nArmor = shipinfo["armor"].get_or<int>((*globals_status)->m_pShipInfo->m_nArmor);
    (*globals_status)->m_pShipInfo->m_nBonusCapacity = shipinfo["bonuscargo"].get_or<int>((*globals_status)->m_pShipInfo->m_nBonusCapacity);
    (*globals_status)->m_pShipInfo->m_fPower = shipinfo["power"].get_or<int>((*globals_status)->m_pShipInfo->m_fPower);
}

bool Player::hasshiparmor()
{
    if (getshipinfo()["armor"] != 0)
        return true;
    return false;
}

bool Player::hasjumpdrive()
{
    int hasdrive = 0;
    uintptr_t address = Offset::SHIP_HASJUMPDRIVE;
    ShipInfo* shipinfo = (*globals_status)->m_pShipInfo;

    __asm {
        mov eax, shipinfo
        call address
        mov hasdrive, eax
    }
    return hasdrive != 0;
}

int Player::getenemieskilled()
{
    return (*globals_status)->m_nEnemiesKilled;
}

void Player::setenemieskilled(int value)
{
    (*globals_status)->m_nEnemiesKilled = value;
}

int Player::getlevel()
{
    return (*globals_status)->m_nPlayerLevel;
}

void Player::setlevel(int value)
{
    (*globals_status)->m_nPlayerLevel = value;
}

int Player::getvisitedstations()
{
    return (*globals_status)->m_nVisitedStations;
}

void Player::setvisitedstations(int value)
{
    (*globals_status)->m_nVisitedStations = value;
}

int Player::getcargosalvagedcount()
{
    return (*globals_status)->m_nCargoSalvaged;
}

void Player::setcargosalvagedcount(int value)
{
    (*globals_status)->m_nCargoSalvaged = value;
}

bool Player::isdocked(void)
{
    if ((*globals_appmanager)->m_nCurrentModule == 5)
        return true;
    return false;
}

int Player::getasteroidsdestroyedcount()
{
    return (*globals_status)->m_nAsteroidsDestroyed;
}

void Player::setasteroidsdestroyedcount(int value)
{
    (*globals_status)->m_nAsteroidsDestroyed = value;
}

void Player::resetgame()
{
    uintptr_t address = Offset::STATUS_RESETGAME;

    __asm {
        call address
    }
}

void Player::setstation(int id)
{
    Station *stationptr = nullptr;
    Globals_status* status = *globals_status;
    uintptr_t getstation_address = Offset::GALAXY_GETSTATION;
    uintptr_t setstation_address = Offset::STATUS_SETSTATION;

    __asm {
        push id
        call getstation_address
        mov stationptr, eax
    }
    if (!stationptr) {
        std::cout << "[-] Station id " << id << " doesn't exist" << std::endl;
        return;
    }
    __asm {
        mov edx, stationptr
        mov ebx, status
        call setstation_address
    }
}

void Player::setequipment(int slot, int itemid)
{
    uintptr_t itemmakeitem_address = Offset::ITEM_MAKEITEM;
    uintptr_t shipsetequipment_address = Offset::SHIP_SETEQUIPMENT;
    Items *itemsarray = reinterpret_cast<Items*>(Offset::GLOBALS_ITEMS);
    int *itemaddrtmp = nullptr;
    ShipInfo *ship = (*globals_status)->m_pShipInfo;

    if (!itemsarray || !ship)
        return;
    int *itemaddr = reinterpret_cast<int*>(itemsarray->items->data[itemid]);
    __asm {
        mov eax, itemaddr
        call itemmakeitem_address
        mov itemaddrtmp, eax
    }
    if (!itemaddrtmp)
        return;
    __asm {
        mov eax, slot
        mov ecx, ship
        mov ebx, itemaddrtmp
        call shipsetequipment_address
    }
}

// TODO: maybe add support for turrets, primary/secondary weapons but it will crash if no weapons point so it's annoying to make that feature
// note to me : if it does weird things maybe add ship::refreshvalue
void Player::addequipmentslot(int itemid)
{
    Items *itemsarray = reinterpret_cast<Items*>(Offset::GLOBALS_ITEMS);

    if (!globals_status || *globals_status == nullptr)
        return;
    if (!itemsarray)
        return;
    SingleItem *item = itemsarray->items->data[itemid];
    if (item->m_nType != 3) {
        std::cout << "[-] The itemid you specified isn't an equipment but probably a primary/secondary weapon which isn't supported" << std::endl;
        return;
    }
    ShipInfo *ship = (*globals_status)->m_pShipInfo;
    if (!ship || !ship->m_pShipSlots || !ship->m_pShipEquipment)
        return;
    int oldequipments = ship->m_pShipSlots->m_nEquipments;
    int newequipments = oldequipments + 1;
    int totalslots = ship->m_pShipSlots->m_nPrimaryWeapons + ship->m_pShipSlots->m_nSecondaryWeapons + ship->m_pShipSlots->m_nTurrets + oldequipments;
    int newslots = totalslots + 1;
    auto *equipmentarray = reinterpret_cast<AEArray<SingleItem*>*>(ship->m_pShipEquipment);
    SingleItem **olddata = equipmentarray->data;
    int oldsize = equipmentarray->size;
    SingleItem **newdata = reinterpret_cast<SingleItem**>(AbyssEngine::memory_allocate(newslots * sizeof(SingleItem*)));
    memset(newdata, 0, newslots * sizeof(SingleItem*));
    if (olddata && oldsize > 0) {
        int copysize = (oldsize < newslots) ? oldsize : newslots;
        memcpy(newdata, olddata, copysize * sizeof(SingleItem*));        
        AbyssEngine::memory_free(olddata);
    }
    equipmentarray->data = newdata;
    equipmentarray->size = newslots;
    equipmentarray->size2 = newslots;
    ship->m_pShipSlots->m_nEquipments = newequipments;
    setequipment(oldequipments, itemid);
}

// no i won't use the real game func
void Player::removeequipmentslot(int itemid)
{
    uintptr_t shiprefreshvalue_address = Offset::SHIP_REFRESHVALUE;

    if (globals_status == nullptr || *globals_status == nullptr)
        return;
    ShipInfo *ship = (*globals_status)->m_pShipInfo;
    if (!ship || !ship->m_pShipSlots || !ship->m_pShipEquipment)
        return;
    int prims = ship->m_pShipSlots->m_nPrimaryWeapons;
    int secs = ship->m_pShipSlots->m_nSecondaryWeapons;
    int turrs = ship->m_pShipSlots->m_nTurrets;
    int equips = ship->m_pShipSlots->m_nEquipments;
    int equip_start = prims + secs + turrs;
    int target = -1;
    auto *equip_arr = reinterpret_cast<AEArray<SingleItem*>*>(ship->m_pShipEquipment);
    if (!equip_arr || !equip_arr->data)
        return;
    for (int i = 0; i < equips; i++) {
        int id = equip_start + i;
        if (id < equip_arr->size && equip_arr->data[id] != nullptr) {
            if (equip_arr->data[id]->m_nID == itemid) {
                target = id;
                break;
            }
        }
    }
    if (target == -1) {
        std::cout << "[-] Item " << itemid << " not found in active equipment slots" << std::endl;
        return;
    }
    AbyssEngine::memory_free(equip_arr->data[target]);
    int total_slots = prims + secs + turrs + equips;
    int new_slots = total_slots - 1;
    SingleItem **new_data = nullptr;
    if (new_slots > 0) {
        new_data = reinterpret_cast<SingleItem**>(AbyssEngine::memory_allocate(new_slots * sizeof(SingleItem*)));
        if (!new_data)
            return;
        memset(new_data, 0, new_slots * sizeof(SingleItem*));
    }
    int dst = 0;
    for (int src = 0; src < total_slots; src++) {
        if (src == target)
            continue; 
        if (dst < new_slots && equip_arr->data) {
            new_data[dst] = equip_arr->data[src];
            dst++;
        }
    }
    SingleItem **old_data = equip_arr->data;
    AbyssEngine::memory_free(old_data);
    equip_arr->data = new_data;
    equip_arr->size = new_slots;
    equip_arr->size2 = new_slots;
    ship->m_pShipSlots->m_nEquipments = equips - 1;
    __asm {
        mov esi, ship
        call shiprefreshvalue_address
    }
}

bool Player::hasequipment(int itemid)
{
    uintptr_t shiphasequipment_address = Offset::SHIP_HASEQUIPMENT;
    bool returnval = false;

    if (!globals_status || *globals_status == nullptr)
        return returnval;
    ShipInfo *ship = (*globals_status)->m_pShipInfo;
    __asm {
        push itemid
        mov eax, ship
        call shiphasequipment_address
        mov returnval, al
    }
    return returnval;
}