#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

void Asset::init()
{
    globals_canvas = MemoryUtils::GetModuleBase("GoF2.exe") + 0x20AE68; // Globals::Canvas
}

std::string Asset::getassetfilepath(unsigned int offset)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_canvas, {0x148, offset, 0xC, 0x0, 0x0});
    return MemoryUtils::ReadString(finaladdr);
}

void Asset::setassetfilepath(unsigned int offset, const std::string value)
{
    uintptr_t finaladdr = MemoryUtils::GetPointerAddress(globals_canvas, {0x148, offset, 0xC, 0x0, 0x0});
    MemoryUtils::WriteString(finaladdr, value);
}