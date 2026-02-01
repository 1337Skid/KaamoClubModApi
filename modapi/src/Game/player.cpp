#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <thread>
#include <chrono>

void Player::init(lua_State* lua_state)
{
    auto start = std::chrono::high_resolution_clock::now();
    uintptr_t base = MemoryUtils::GetModuleBase("GoF2.exe");

    while (globals_status == nullptr) {
        globals_status = *reinterpret_cast<Globals_status**>(base + 0x20AD6C); // Globals::status
        if (globals_status == nullptr) 
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    }

    while (globals_appmanager == nullptr) {
        globals_appmanager = *reinterpret_cast<Globals_appManager**>(base + 0x20AEFC); // Globals::appManager
        if (globals_appmanager == nullptr)
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    lstate = lua_state;
}

int Player::getmoney()
{
    return globals_status->m_nMoney;
}

void Player::setmoney(int value)
{
    globals_status->m_nMoney = value;
}

void Player::togglecloaking()
{
    uintptr_t address = 0x004ABA3B;
    MGame* mgame = reinterpret_cast<MGame*>(globals_appmanager->m_pCurrentModule);
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
    uintptr_t address = 0x4AC590;
    MGame* mgame = reinterpret_cast<MGame*>(globals_appmanager->m_pCurrentModule);
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

    shipinfo["id"] = globals_status->m_pShipInfo->m_nID;
    shipinfo["maxhealth"] = globals_status->m_pShipInfo->m_nMaxHealth;
    shipinfo["baseprice"] = globals_status->m_pShipInfo->m_nBasePrice;
    shipinfo["maxcargo"] = globals_status->m_pShipInfo->m_nMaxCapacity;
    shipinfo["cargo"] = globals_status->m_pShipInfo->m_nCurrentCapacity;
    shipinfo["price"] = globals_status->m_pShipInfo->m_nPrice;
    shipinfo["controlmultiplier"] = globals_status->m_pShipInfo->m_fControlMultiplier;
    shipinfo["armor"]  = globals_status->m_pShipInfo->m_nArmor;
    shipinfo["bonuscargo"] = globals_status->m_pShipInfo->m_nBonusCapacity;
    shipinfo["power"] = globals_status->m_pShipInfo->m_fPower;
    return shipinfo;
}

void Player::setshipinfo(sol::table shipinfo)
{
    if (!shipinfo) {
        std::cout << "[-] Shipinfo can't be null" << std::endl;
        return;
    }
    globals_status->m_pShipInfo->m_nID = shipinfo["id"].get_or<int>(globals_status->m_pShipInfo->m_nID);
    globals_status->m_pShipInfo->m_nMaxHealth = shipinfo["maxhealth"].get_or<int>(globals_status->m_pShipInfo->m_nMaxHealth);
    globals_status->m_pShipInfo->m_nBasePrice = shipinfo["baseprice"].get_or<int>(globals_status->m_pShipInfo->m_nBasePrice);
    globals_status->m_pShipInfo->m_nMaxCapacity = shipinfo["maxcargo"].get_or<int>(globals_status->m_pShipInfo->m_nMaxCapacity);
    globals_status->m_pShipInfo->m_nCurrentCapacity = shipinfo["cargo"].get_or<int>(globals_status->m_pShipInfo->m_nCurrentCapacity);
    globals_status->m_pShipInfo->m_nPrice = shipinfo["price"].get_or<int>(globals_status->m_pShipInfo->m_nPrice);
    globals_status->m_pShipInfo->m_fControlMultiplier = shipinfo["controlmultiplier"].get_or<int>(globals_status->m_pShipInfo->m_fControlMultiplier);
    globals_status->m_pShipInfo->m_nArmor = shipinfo["armor"].get_or<int>(globals_status->m_pShipInfo->m_nArmor);
    globals_status->m_pShipInfo->m_nBonusCapacity = shipinfo["bonuscargo"].get_or<int>(globals_status->m_pShipInfo->m_nBonusCapacity);
    globals_status->m_pShipInfo->m_fPower = shipinfo["power"].get_or<int>(globals_status->m_pShipInfo->m_fPower);
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
    uintptr_t address = 0x004C2F11;
    ShipInfo* shipinfo = globals_status->m_pShipInfo;

    __asm {
        mov eax, shipinfo
        call address
        mov hasdrive, eax
    }
    return hasdrive != 0;
}

int Player::getenemieskilled()
{
    return globals_status->m_nEnemiesKilled;
}

void Player::setenemieskilled(int value)
{
    globals_status->m_nEnemiesKilled = value;
}

int Player::getlevel()
{
    return globals_status->m_nPlayerLevel;
}

void Player::setlevel(int value)
{
    globals_status->m_nPlayerLevel = value;
}

int Player::getvisitedstations()
{
    return globals_status->m_nVisitedStations;
}

void Player::setvisitedstations(int value)
{
    globals_status->m_nVisitedStations = value;
}

int Player::getcargosalvagedcount()
{
    return globals_status->m_nCargoSalvaged;
}

void Player::setcargosalvagedcount(int value)
{
    globals_status->m_nCargoSalvaged = value;
}

bool Player::isdocked(void)
{
    if (globals_appmanager->m_nCurrentModule == 5)
        return true;
    return false;
}

int Player::getasteroidsdestroyedcount()
{
    return globals_status->m_nAsteroidsDestroyed;
}

void Player::setasteroidsdestroyedcount(int value)
{
    globals_status->m_nAsteroidsDestroyed = value;
}