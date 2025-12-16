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
    money = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C;
    maxcargo = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C;
    cargo = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C;
    shiparmor = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AD6C;
}

int Player::getmoney()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(money, {0x174});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setmoney(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(money, {0x174});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getmaxcargo()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(maxcargo, {0x154, 0x0});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setmaxcargo(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(maxcargo, {0x154, 0x0});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getcargo()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(maxcargo, {0x154, 0x10});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setcargo(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(maxcargo, {0x154, 0x10});
    MemoryUtils::Write<int>(finaladdr, value);
}

int Player::getshiparmor()
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(shiparmor, {0x154, 0x20});
    return MemoryUtils::Read<int>(finaladdr);
}

void Player::setshiparmor(int value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(shiparmor, {0x154, 0x20});
    MemoryUtils::Write<int>(finaladdr, value);
}

bool Player::hasshiparmor()
{
    if (getshiparmor() != 0)
        return true;
    return false;
}