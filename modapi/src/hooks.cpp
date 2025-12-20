#include "patches.h"
#include "abyssengine.h"
#include "hooks.h"
#include <Game/system.h>

Hooks::globals_init Hooks::oldglobals_init = nullptr;

void Hooks::injectsystems()
{
    auto* galaxy = *(Galaxy**)GLOBALS_GALAXY;

    if (!galaxy || !galaxy->systems) {
        std::cout << "[-] Failed to get Globals::Galaxy" << std::endl;
        return;
    }

    if (System::created_systems.empty())
        return;

    auto* systems = galaxy->systems;
    SingleSystem* wolf = nullptr;

    for (uint32_t i = 0; i < systems->size; ++i) {
        auto* s = systems->data[i];
        if (s && s->name.text && wcscmp(s->name.text, L"Wolf-Reiser") == 0) {
            wolf = s;
            break;
        }
    }

    const uint32_t sys_count = systems->size;
    const uint32_t add_sys_count = (uint32_t)System::created_systems.size();
    const uint32_t new_sys_count = sys_count + add_sys_count;

    auto* new_sys_array = (AEArray<SingleSystem*>*)VirtualAlloc(nullptr, sizeof(AEArray<SingleSystem*>),MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    new_sys_array->size = new_sys_array->size2 = new_sys_count;
    new_sys_array->data = (SingleSystem**)VirtualAlloc(nullptr, sizeof(SingleSystem*) * new_sys_count, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    memcpy(new_sys_array->data, systems->data,sizeof(SingleSystem*) * sys_count);

    for (size_t i = 0; i < System::created_systems.size(); ++i) {
        const uint32_t id = sys_count + (uint32_t)i;

        SingleSystem copy = *wolf;
        const SingleSystem& newsys = System::created_systems[i];

        copy.id = id;
        copy.pos = newsys.pos;
        copy.name = newsys.name;
        copy.jumpgate_station_id = newsys.jumpgate_station_id;
        copy.starts_unlocked = true;

        copy.linked_system_ids = AbyssEngine::newarray<uint32_t>(1);
        copy.linked_system_ids->data[0] = id;

        copy.station_ids = AbyssEngine::newarray<uint32_t>(1);
        copy.station_ids->data[0] = 109;

        auto* newsysdata = (SingleSystem*)VirtualAlloc(nullptr, sizeof(SingleSystem), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        memcpy(newsysdata, &copy, sizeof(SingleSystem));
        new_sys_array->data[sys_count + i] = newsysdata;
    }

    galaxy->systems = new_sys_array;

    Patches::patchstarmap((uint8_t)new_sys_count);
    Patches::patchloadstations(110); // loaded manually into stations.bin atm

    System::created_systems.clear();
}

uintptr_t __stdcall Hooks::globals_init_hook(uintptr_t a, uintptr_t b, uintptr_t c)
{
    auto old = oldglobals_init(a, b, c);
    static bool one = false;
    if (!one) {
        one = true;
        CreateThread(nullptr, 0, [](LPVOID)->DWORD {
            std::cout << "[+] Globals::init hooked successfully" << std::endl;
            Hooks::injectsystems();
            return 0;
        }, nullptr, 0, nullptr);
    }
    return old;
}

void Hooks::init()
{
    std::cout << "[*] Starting hooks..." << std::endl;
    MH_Initialize();
    MH_CreateHook((LPVOID)GLOBALS_INIT_ADDR, &globals_init_hook, (LPVOID*)&oldglobals_init);
    MH_EnableHook((LPVOID)GLOBALS_INIT_ADDR);
}