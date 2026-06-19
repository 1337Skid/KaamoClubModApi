#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/engine.h>

void Engine::init()
{
    globals_appmanager = reinterpret_cast<Globals_appManager**>(Offset::GLOBALS_APPMANAGER);
}

void Engine::setcurrentapplicationmodule(int id)
{
    Globals_appManager *appmanager = *(globals_appmanager);
    uintptr_t address = Offset::ABYSSENGINE_APPLICATIONMANAGER_SETCURRENTAPPLICATIONMODULE;

    __asm {
        mov ecx, appmanager
        mov edx, id
        push esi
        mov eax, ecx
        mov esi, edx
        mov ecx, address
        call ecx
        pop esi
    }
}