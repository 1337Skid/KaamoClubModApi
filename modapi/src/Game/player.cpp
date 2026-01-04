#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <thread>
#include <chrono>

void Player::init()
{
    auto start = std::chrono::high_resolution_clock::now();
    uintptr_t base = MemoryUtils::GetModuleBase("GoF2.exe");

    while (globals_status == nullptr) {
        globals_status = *reinterpret_cast<Globals_status**>(base + 0x20AD6C); // Globals::status
        if (globals_status == nullptr) 
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    while (globals_appmanager == nullptr) {
        globals_appmanager = *reinterpret_cast<Globals_appManager**>(base + 0x20AEFC); // Globals::appManager
        if (globals_appmanager == nullptr)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("[+] Player Initialization took: %lld ms\n", duration);
}

int Player::getmoney()
{
    return globals_status->m_nMoney;
}

void Player::setmoney(int value)
{
    globals_status->m_nMoney = value;
}

int Player::getmaxcargo()
{
    return globals_status->m_pShipInfo->m_nMaxCapacity;
}

void Player::setmaxcargo(int value)
{
    globals_status->m_pShipInfo->m_nMaxCapacity;
}

int Player::getcargo()
{
    return globals_status->m_pShipInfo->m_nCurrentCapacity;
}

void Player::setcargo(int value)
{
    globals_status->m_pShipInfo->m_nCurrentCapacity = value;
}

int Player::getshiparmor()
{
    return globals_status->m_pShipInfo->m_nArmor;
}

void Player::setshiparmor(int value)
{
    globals_status->m_pShipInfo->m_nArmor = value;
}

bool Player::hasshiparmor()
{
    if (getshiparmor() != 0)
        return true;
    return false;
}

int Player::getmaxshiphealth()
{
    return globals_status->m_pShipInfo->m_nMaxShipHealth;
}

void Player::setmaxshiphealth(int value)
{
    globals_status->m_pShipInfo->m_nMaxShipHealth = value;
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