#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/level.h>
#include <Game/kiplayer.h>
#include <thread>
#include <chrono>
#include "patches.h"
#include "offset.h"
#include "abyssengine.h"

KIPlayer::KIPlayer(int *ptr) : _ptr(ptr) {};
KIPlayer::~KIPlayer() {};

std::string KIPlayer::tostring(void)
{
    std::ostringstream oss;
    oss << "KIPlayer(0x" << reinterpret_cast<void*>(_ptr) << ")";
    return oss.str();
}

// TODO: check if it's playerfighter because only playerfighter can have a route (well it will crash for others players class)
void KIPlayer::setroute(int *routeptr)
{
    uintptr_t address = Offset::KIPLAYER_SETROUTE;
    
    if (!_ptr || !routeptr) {
        std::cout << "KiPlayer or route is null" << std::endl;
        return;
    }
    void *kiplayer = reinterpret_cast<void*>(_ptr);
    void *route = reinterpret_cast<void*>(routeptr);
    __asm {
        mov edi, route
        mov esi, kiplayer
        call address
    }
}