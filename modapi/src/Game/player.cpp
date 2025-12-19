#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

void Player::init()
{
    globals_status = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C; // Globals::status
    globals_appmanager = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AEFC; // Globals::appManager
}

int Player::getmoney()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x174});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setmoney(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x174});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getmaxcargo()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x0});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setmaxcargo(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x0});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getcargo()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x10});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setcargo(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x10});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getshiparmor()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x20});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setshiparmor(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x20});
    MemoryUtils::Write<int>(finaladdr, value);
}

bool Player::hasshiparmor()
{
    if (getshiparmor() != 0)
        return true;
    return false;
}

int Player::getmaxshiphealth()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x4});
    return MemoryUtils::Read<int>(finaladdr); 
}

void Player::setmaxshiphealth(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x154, 0x4});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getenemieskilled()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x188});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setenemieskilled(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x188});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getlevel()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x190});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setlevel(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x190});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getvisitedstations()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x198});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setvisitedstations(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x198});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getjumpgateusedcount()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x198});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setjumpgateusedcount(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x198});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getcargotookcount()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x1A8});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setcargotookcount(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_status, {0x1A8});
    MemoryUtils::Write<int>(finaladdr, value);
}

bool Player::isdocked(void)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_appmanager, {0x58});
    int isdocked = MemoryUtils::Read<int>(finaladdr);

    if (isdocked == 5)
        return true;
    return false;
}