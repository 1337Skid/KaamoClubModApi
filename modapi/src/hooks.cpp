#include "patches.h"
#include "abyssengine.h"
#include "hooks.h"
#include <Game/system.h>

Hooks::globals_init Hooks::oldglobals_init = nullptr;
Hooks::fileread_loadstationbinaryfromid Hooks::old_filereadloadstationbinaryfromid = nullptr;
Hooks::fileread_loadstationbinary Hooks::old_filereadloadstationbinary = nullptr;

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
    static bool called_once = false;
    if (!called_once) {
        called_once = true;
        CreateThread(nullptr, 0, [](LPVOID)->DWORD {
            std::cout << "[+] Globals::init hooked successfully" << std::endl;
            Hooks::injectsystems();
            return 0;
        }, nullptr, 0, nullptr);
    }
    return old;
}

uintptr_t __stdcall Hooks::fileread_loadstationbinaryfromid_hook(const uint16_t* a)
{
    //uint16_t newval = 108;
    //auto old = old_filereadloadstationbinaryfromid(&newval);
    auto old = old_filereadloadstationbinaryfromid(a);
    //std::cout << "called" << std::endl;
    //std::cout << *a << std::endl;
    if (a && *a > 108) {
        std::cout << "[*] Should teleport to our custom station" << std::endl;
        Sleep(13000);
        // TODO: return the array of our custom station with the specific id
    }
    //std::cout << "[*] FileRead::loadStationBinaryFromId ptr = " << a << std::endl;
    //std::cout << "[*] FileRead::loadStationBinaryFromId id = " << *a << std::endl;
    return old;
}

uintptr_t __stdcall Hooks::fileread_loadstationbinary_hook(SingleSystem* system)
{
    auto* old_array = reinterpret_cast<AEArray<SingleStation*>*>(old_filereadloadstationbinary(system));

    if (system->id != 27)
        return reinterpret_cast<uintptr_t>(old_array);

    std::cout << "[*] System id: " << system->id << std::endl;
    SingleStation* first_station = old_array->data[0];
    if (!first_station)
        return reinterpret_cast<uintptr_t>(old_array);

    std::cout << first_station << std::endl;

    return reinterpret_cast<uintptr_t>(old_array);
}

void Hooks::init()
{
    std::cout << "[*] Starting hooks..." << std::endl;
    MH_Initialize();
    MH_CreateHook((LPVOID)GLOBALS_INIT_ADDR, &globals_init_hook, (LPVOID*)&oldglobals_init);
    MH_CreateHook((LPVOID)FILEREAD_LOADSTATIONBINARYFROMID, &fileread_loadstationbinaryfromid_hook, (LPVOID*)&old_filereadloadstationbinaryfromid);
    MH_CreateHook((LPVOID)FILEREAD_LOADSTATIONBIRARY, &fileread_loadstationbinary_hook, (LPVOID*)&old_filereadloadstationbinary);
    MH_EnableHook((LPVOID)GLOBALS_INIT_ADDR);
    MH_EnableHook((LPVOID)FILEREAD_LOADSTATIONBINARYFROMID);
    MH_EnableHook((LPVOID)FILEREAD_LOADSTATIONBIRARY);
}