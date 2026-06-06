#include "patches.h"
#include "abyssengine.h"
#include "hooks.h"
#include "hookcontext.h"
#include <Game/system.h>
#include <Game/level.h>
#include <Game/ship.h>
#include <iostream>
#include <vector>
#include <cmath>

void Hooks::injectitems()
{
    Items* itemsarray = reinterpret_cast<Items*>(Offset::GLOBALS_ITEMS);
    AEArray<SingleItem*>* old_array = itemsarray->items;
    int newcount = old_array->size + static_cast<int>(Item::created_items.size());
    AEArray<SingleItem*>* new_array = reinterpret_cast<AEArray<SingleItem*>*>(AbyssEngine::memory_allocate(sizeof(AEArray<SingleItem*>)));
    new_array->size = newcount;
    new_array->size2 = newcount;
    new_array->data = (SingleItem**)AbyssEngine::memory_allocate(sizeof(SingleItem*) * newcount);
    memcpy(new_array->data, old_array->data, sizeof(SingleItem*) * old_array->size);
    for (int i = 0; i < static_cast<int>(Item::created_items.size()); i++) {
        SingleItem* item = (SingleItem*)AbyssEngine::memory_allocate(sizeof(SingleItem));        
        *item = Item::created_items[i].item;
        new_array->data[old_array->size + i] = item;
    }
    itemsarray->items = new_array;
    // I'm not freeing old_array->data[i]->m_pItemInfo array because I did a memcpy of old array to new array and I don't want to realloc every item info it's kinda useless
    AbyssEngine::memory_free(old_array->data);
    AbyssEngine::memory_free(old_array);
}

void Hooks::injectships()
{
    if (Ship::created_ships.empty())
        return;
    auto& ships = **reinterpret_cast<AEArray<int*>**>(Offset::GLOBALS_SHIPS);
    int old = ships.size; 
    int total = old + static_cast<int>(Ship::created_ships.size()); 
    auto** newship_data = reinterpret_cast<int**>(AbyssEngine::memory_allocate(sizeof(int*) * total));
    if (old > 0 && ships.data != nullptr)
        std::memcpy(newship_data, ships.data, sizeof(int*) * old);
    auto* newhangaroffsets_one = reinterpret_cast<DWORD*>(AbyssEngine::memory_allocate(sizeof(DWORD) * total));
    auto* newhangaroffsets_two = reinterpret_cast<DWORD*>(AbyssEngine::memory_allocate(sizeof(DWORD) * total));
    auto* meshes = reinterpret_cast<WORD*>(AbyssEngine::memory_allocate(sizeof(WORD) * total));
    auto* nolights = reinterpret_cast<WORD*>(AbyssEngine::memory_allocate(sizeof(WORD) * total));
    auto* addlights = reinterpret_cast<WORD*>(AbyssEngine::memory_allocate(sizeof(WORD) * total));
    auto* lods = reinterpret_cast<uint8_t*>(AbyssEngine::memory_allocate(12 * total));
    auto* lightlods = reinterpret_cast<uint8_t*>(AbyssEngine::memory_allocate(12 * total));
    auto* ui_meshes_1 = reinterpret_cast<WORD*>(AbyssEngine::memory_allocate(sizeof(WORD) * total));
    auto* ui_meshes_2 = reinterpret_cast<WORD*>(AbyssEngine::memory_allocate(sizeof(WORD) * total));
    std::memcpy(newhangaroffsets_one, reinterpret_cast<void*>(Offset::SHIP_HANGAR_OFFSETS), sizeof(DWORD) * old);
    std::memcpy(newhangaroffsets_two, reinterpret_cast<void*>(Offset::SHIP_HANGAR_OFFSETS2), sizeof(DWORD) * old);
    std::memcpy(meshes, reinterpret_cast<void*>(Offset::SHIP_MESHES), sizeof(WORD) * old);
    std::memcpy(nolights, reinterpret_cast<void*>(Offset::SHIP_MESHES_NO_LIGHT), sizeof(WORD) * old);
    std::memcpy(addlights, reinterpret_cast<void*>(Offset::SHIP_MESHES_ADD_LIGHTS), sizeof(WORD) * old);
    std::memcpy(lods, reinterpret_cast<void*>(Offset::SHIP_LODS), 12 * old);
    std::memcpy(lightlods, reinterpret_cast<void*>(Offset::SHIP_LIGHT_LODS), 12 * old);
    std::memcpy(ui_meshes_1, reinterpret_cast<void*>(0x0052F288), sizeof(WORD) * old);
    std::memcpy(ui_meshes_2, reinterpret_cast<void*>(0x0052F2E0), sizeof(WORD) * old);
    auto* copyship_example = reinterpret_cast<ShipInfo*>(ships.data[0]); // we copy the first ship for a good example (we'll edit the stats etc...)
    for (int i = 0; i < Ship::created_ships.size(); i++) {
        int shipid = old + static_cast<int>(i);
        ShipInfo* newship = reinterpret_cast<ShipInfo*>(AbyssEngine::memory_allocate(120));
        std::memcpy(newship, copyship_example, 120);
        ShipSlots* newslots = reinterpret_cast<ShipSlots*>(AbyssEngine::memory_allocate(sizeof(ShipSlots)));
        std::memcpy(newslots, copyship_example->m_pShipSlots, sizeof(ShipSlots));
        newship->m_pShipSlots = newslots;
        newship->m_nID = shipid;
        newship->m_nMaxHealth = Ship::created_ships[i].armor;
        newship->m_nBasePrice = Ship::created_ships[i].baseprice;
        newship->m_nPrice = Ship::created_ships[i].baseprice;
        newship->m_nMaxCapacity = Ship::created_ships[i].maxcargo;
        newship->m_fControlMultiplier = Ship::created_ships[i].handling;
        newship->m_pShipSlots->m_nPrimaryWeapons = Ship::created_ships[i].primaryslots;
        newship->m_pShipSlots->m_nSecondaryWeapons = Ship::created_ships[i].secondaryslots;
        newship->m_pShipSlots->m_nTurrets = Ship::created_ships[i].turretslots;
        newship->m_pShipSlots->m_nEquipments = Ship::created_ships[i].equipmentslots;
        newship_data[shipid] = reinterpret_cast<int*>(newship);
        newhangaroffsets_one[shipid] = Ship::created_ships[i].hangar_y;
        newhangaroffsets_two[shipid] = Ship::created_ships[i].hangar_y;
        ui_meshes_1[shipid] = Ship::created_ships[i].lod0;
        ui_meshes_2[shipid] = Ship::created_ships[i].lod0;
        meshes[shipid] = Ship::created_ships[i].lod0;
        nolights[shipid] = -1;
        addlights[shipid] = -1;
        auto* current_lod = lods + (shipid * 12);
        *reinterpret_cast<DWORD*>(current_lod + 0) = Ship::created_ships[i].lod0;
        *reinterpret_cast<DWORD*>(current_lod + 4) = Ship::created_ships[i].lod1;
        *reinterpret_cast<DWORD*>(current_lod + 8) = Ship::created_ships[i].lod2;
        auto* current_lightlods = lightlods + (shipid * 12);
        *reinterpret_cast<WORD*>(current_lightlods + 0) = -1;
        *reinterpret_cast<WORD*>(current_lightlods + 2) = 0;
        *reinterpret_cast<WORD*>(current_lightlods + 4) = -1;
        *reinterpret_cast<WORD*>(current_lightlods + 6) = 0;
        *reinterpret_cast<WORD*>(current_lightlods + 8) = -1;
        *reinterpret_cast<WORD*>(current_lightlods + 10) = 0;
    }
    ships.data = newship_data;
    ships.size = total;
    ships.size2 = total;
    DWORD oldp;
    // we are going to use our own array in the getshipgroup function instead of the rdata arrays
    // hangar offset
    VirtualProtect(reinterpret_cast<LPVOID>(0x0040512E), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x0040512E) = reinterpret_cast<DWORD>(newhangaroffsets_one);
    VirtualProtect(reinterpret_cast<LPVOID>(0x00472CC0), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x00472CC0) = reinterpret_cast<DWORD>(newhangaroffsets_two);
    VirtualProtect(reinterpret_cast<LPVOID>(0x004730A4), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x004730A4) = reinterpret_cast<DWORD>(newhangaroffsets_two);
    // meshes array
    VirtualProtect(reinterpret_cast<LPVOID>(0x0044D58F), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x0044D58F) = reinterpret_cast<DWORD>(meshes);
    VirtualProtect(reinterpret_cast<LPVOID>(0x0044D5BD), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x0044D5BD) = reinterpret_cast<DWORD>(nolights);
    VirtualProtect(reinterpret_cast<LPVOID>(0x0044D64E), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x0044D64E) = reinterpret_cast<DWORD>(addlights);
    VirtualProtect(reinterpret_cast<LPVOID>(0x0044D674), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x0044D674) = reinterpret_cast<DWORD>(addlights);
    // available lods array
    VirtualProtect(reinterpret_cast<LPVOID>(0x0044D701), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x0044D701) = reinterpret_cast<DWORD>(lods);
    VirtualProtect(reinterpret_cast<LPVOID>(0x0044D834), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x0044D834) = reinterpret_cast<DWORD>(lightlods);
    // UI ships mesh preview
    VirtualProtect(reinterpret_cast<LPVOID>(0x00481059), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x00481059) = reinterpret_cast<DWORD>(ui_meshes_1);
    VirtualProtect(reinterpret_cast<LPVOID>(0x00481085), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x00481085) = reinterpret_cast<DWORD>(ui_meshes_2);
    VirtualProtect(reinterpret_cast<LPVOID>(0x004810A5), 4, PAGE_EXECUTE_READWRITE, &oldp);
    *reinterpret_cast<DWORD*>(0x004810A5) = reinterpret_cast<DWORD>(ui_meshes_2);
}

void Hooks::injectsystemsandstations()
{
    auto* galaxy = *reinterpret_cast<Galaxy**>(Offset::GLOBALS_GALAXY);
    if (!galaxy || !galaxy->systems || System::created_systems.empty())
        return;
    auto* systems = galaxy->systems;
    const uint32_t sys_count = systems->size;
    const uint32_t add_sys_count = static_cast<uint32_t>(System::created_systems.size());
    const uint32_t new_sys_count = sys_count + add_sys_count;
    auto* new_sys_array = reinterpret_cast<AEArray<SingleSystem*>*>(AbyssEngine::memory_allocate(sizeof(AEArray<SingleSystem*>)));
    new_sys_array->size = new_sys_array->size2 = new_sys_count;
    new_sys_array->data = reinterpret_cast<SingleSystem**>(AbyssEngine::memory_allocate(sizeof(SingleSystem*) * new_sys_count));

    memcpy(new_sys_array->data, systems->data, sizeof(SingleSystem*) * sys_count);
    for (size_t i = 0; i < System::created_systems.size(); ++i) {
        const uint32_t sysid = sys_count + static_cast<uint32_t>(i);
        const SingleSystem& src = System::created_systems[i];
        for (auto& st : Station::created_stations) {
            if (st.systemid == src.id)
                st.systemid = sysid;
        }
        auto* newsys = reinterpret_cast<SingleSystem*>(AbyssEngine::memory_allocate(sizeof(SingleSystem)));
        memset(newsys, 0, sizeof(SingleSystem));
        newsys->id = sysid;
        newsys->name = AbyssEngine::newstring(src.name.text); 
        newsys->pos = src.pos;
        newsys->risk = src.risk;
        newsys->faction = src.faction;
        newsys->texture_id = src.texture_id;
        newsys->starts_unlocked = true;
        std::vector<uint32_t> matching_ids;
        for (const auto& st : Station::created_stations) {
            if (st.systemid == sysid)
                matching_ids.push_back(st.id);
        }
        if (!matching_ids.empty()) {
            newsys->station_ids = AbyssEngine::newarray<uint32_t>(static_cast<int>(matching_ids.size()));
            for (size_t j = 0; j < matching_ids.size(); ++j)
                newsys->station_ids->data[j] = matching_ids[j];
            if (src.linked_system_ids != nullptr)
                newsys->jumpgate_station_id = matching_ids[0];
        }
        uint32_t target_link_id = 0; 
        bool has_link = false;
        if (src.linked_system_ids != nullptr && src.linked_system_ids->size > 0) {
            newsys->linked_system_ids = AbyssEngine::newarray<uint32_t>(1);
            target_link_id = src.linked_system_ids->data[0];
            newsys->linked_system_ids->data[0] = target_link_id;
            has_link = true;
        }
        new_sys_array->data[sysid] = newsys;
        if (has_link && target_link_id < new_sys_count) {
            SingleSystem* target_sys = new_sys_array->data[target_link_id];
            if (target_sys) {
                uint32_t old_link_count = (target_sys->linked_system_ids) ? target_sys->linked_system_ids->size : 0;
                uint32_t new_link_count = old_link_count + 1;
                auto* new_links = AbyssEngine::newarray<uint32_t>(new_link_count);
                if (old_link_count > 0)
                    memcpy(new_links->data, target_sys->linked_system_ids->data, sizeof(uint32_t) * old_link_count);
                new_links->data[old_link_count] = sysid;
                target_sys->linked_system_ids = new_links;
            }
        }
    }
    galaxy->systems = new_sys_array;
    Patches::patchstarmap(static_cast<uint8_t>(new_sys_count)); 
    System::created_systems.clear();
}

uintptr_t __stdcall Hooks::fileread_loadstationbinary_hook(SingleSystem* system)
{
    AEArray<SingleStation*>* old_array = reinterpret_cast<AEArray<SingleStation*>*>(old_filereadloadstationbinary(system));

    if (system) {
        std::vector<const SingleStation*> matchlist;

        for (const auto& st : Station::created_stations) {
            if (st.systemid == system->id) {
                matchlist.push_back(&st);
            }
        }
        if (!matchlist.empty()) {
            int count = static_cast<int>(matchlist.size());
            auto* new_array = reinterpret_cast<AEArray<SingleStation*>*>(AbyssEngine::memory_allocate(sizeof(AEArray<SingleStation*>)));
            if (new_array) {
                new_array->data = reinterpret_cast<SingleStation**>(AbyssEngine::memory_allocate(sizeof(SingleStation*) * count));
                new_array->size = count; 
                new_array->size2 = count;

                for (size_t i = 0; i < matchlist.size(); ++i) {
                    auto* cloned = reinterpret_cast<SingleStation*>(AbyssEngine::memory_allocate(sizeof(SingleStation)));
                    if (cloned) {
                        memcpy(cloned, matchlist[i], sizeof(SingleStation));
                        cloned->name = AbyssEngine::newstring(matchlist[i]->name.text);
                    }
                    new_array->data[i] = cloned;
                }
                return reinterpret_cast<uintptr_t>(new_array);
            }
        }
    }
    return reinterpret_cast<uintptr_t>(old_array);
}

uintptr_t __stdcall Hooks::fileread_loadstationbinaryfromid_hook(const uint16_t* id)
{
    if (!id)
        return old_filereadloadstationbinaryfromid(id);
    uint32_t requested_id = static_cast<uint32_t>(*id);

    for (const auto& st : Station::created_stations) {
        if (st.id == requested_id) {
            auto* new_array = reinterpret_cast<AEArray<SingleStation*>*>(AbyssEngine::memory_allocate(sizeof(AEArray<SingleStation*>)));
            if (new_array) {
                new_array->data = reinterpret_cast<SingleStation**>(AbyssEngine::memory_allocate(sizeof(SingleStation*) * 1));
                new_array->size = 1; 
                new_array->size2 = 1;
                auto* cloned = reinterpret_cast<SingleStation*>(AbyssEngine::memory_allocate(sizeof(SingleStation)));
                memcpy(cloned, &st, sizeof(SingleStation));
                new_array->data[0] = cloned;
                return reinterpret_cast<uintptr_t>(new_array);
            }
        }
    }
    return old_filereadloadstationbinaryfromid(id);
}

uintptr_t __stdcall Hooks::globals_init_hook(uintptr_t a, uintptr_t b, uintptr_t c)
{
    auto old = oldglobals_init(a, b, c);
    static bool called_once = false;

    if (!called_once) {
        called_once = true;
        CreateThread(nullptr, 0, [](LPVOID)->DWORD {
            Patches::patchmissions(100);
            Hooks::injectitems();
            EventManager::trigger_hook<GlobalsInitContext>("Globals::init"); // no ctx:call() on the lua side because the game will crash if globals::init is stopped so it makes no sense to do that
            Hooks::injectships();
            Hooks::injectsystemsandstations();
            for (auto& asset : Asset::created_earlyassets) {
                if (!asset.path.empty()) {
                    Asset::next_texture_id = asset.id;
                    Asset::createtexture(asset.path);
                } else {
                    Asset::next_sprite_id = asset.id;
                    Asset::createsprite(asset.textureid, asset.regionid);
                }
            }
            Asset::created_earlyassets.clear();
        return 0;
        }, nullptr, 0, nullptr);
    }
    return old;
}

bool __fastcall Hooks::standing_isenemy_hook(uintptr_t standing, int race)
{
    return true;
}

void __fastcall Hooks::abyssengine_paintcanvas_setcolor_hook(uintptr_t paintcanvas)
{
    unsigned int hexcolor;

    // the game puts the color in eax......
    __asm {
        mov hexcolor, eax
    }
    //hexcolor = 0x00ff00ff; // r,g,b,a | ff = 255
    __asm {
        mov ecx, paintcanvas
        mov eax, hexcolor
        call old_abyssenginepaintcanvassetcolor
    }
}

AEString* __fastcall Hooks::gametext_gettext_hook()
{
    void* returnaddr = nullptr;
    int id;
    AEString* result = nullptr;

    __asm {
        mov id, eax
        mov edx, [ebp + 4]
        mov returnaddr, edx
    }
    // Custom radio messages texts
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x4bcc59 && !Level::created_radiomessages.empty()) {
        static AEString customstring;
        if (Level::created_radiomessages.size() == 1) {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[0].name.c_str());
            customstring.size = static_cast<uint32_t>(Level::created_radiomessages[0].name.length());
        } else {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[Level::created_radiomessages.size() - 1].name.c_str());
            customstring.size = static_cast<uint32_t>(Level::created_radiomessages[Level::created_radiomessages.size() - 1].name.length());
        }
        return &customstring;
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x4bca93 && !Level::created_radiomessages.empty()) {
        static AEString customstring;
        if (Level::created_radiomessages.size() == 1) {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[0].content.c_str());
            customstring.size = static_cast<uint32_t>(Level::created_radiomessages[0].content.length());
        } else {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[Level::created_radiomessages.size() - 1].content.c_str());
            customstring.size = static_cast<uint32_t>(Level::created_radiomessages[Level::created_radiomessages.size() - 1].content.length());
        }
        return &customstring;
    }
    // The return addresses of every GameText::GetText() items name, useful to not overwrite in others strings
    // TODO: find in game npc cargo hold returnaddr
    // TODO: find crafting done returnaddr
    // Custom items texts
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x483331 || reinterpret_cast<uintptr_t>(returnaddr) == 0x454c4e || reinterpret_cast<uintptr_t>(returnaddr) == 0x44edae || reinterpret_cast<uintptr_t>(returnaddr) == 0x4c9725 || reinterpret_cast<uintptr_t>(returnaddr) == 0x4cc0bc || reinterpret_cast<uintptr_t>(returnaddr) == 0x48d8b1 || reinterpret_cast<uintptr_t>(returnaddr) == 0x458618 || reinterpret_cast<uintptr_t>(returnaddr) == 0x4585C1 || reinterpret_cast<uintptr_t>(returnaddr) == 0x455854 || reinterpret_cast<uintptr_t>(returnaddr) == 0x453644) {
        for (const auto& item : Item::created_items) {
            if (id == item.id + 1247) { // + 1247 bcz the game decided so don't ask me why
                static AEString customstring;
                customstring.text = const_cast<wchar_t*>(item.name.c_str());
                customstring.size = static_cast<uint32_t>(item.name.length());
                return &customstring;
            }
        }
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x48307b) {
        for (const auto& item : Item::created_items) {
            if (id == item.id + 1014) { // + 1014 bcz the game decided
                static AEString customstring;
                customstring.text = const_cast<wchar_t*>(item.description.c_str());
                customstring.size = static_cast<uint32_t>(item.description.length());
                return &customstring;
            }
        }
    }
    // Custom dialogue window message
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x4071bf && !Level::created_dialoguemessages.empty()) {
        static AEString customstring;
        customstring.text = const_cast<wchar_t*>(Level::created_dialoguemessages[Level::current_dialogue_id].name.c_str());
        customstring.size = static_cast<uint32_t>(Level::created_dialoguemessages[Level::current_dialogue_id].name.length());
        return &customstring;
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x407279 && !Level::created_dialoguemessages.empty()) {
        static AEString customstring;
        customstring.text = const_cast<wchar_t*>(Level::created_dialoguemessages[Level::current_dialogue_id].content.c_str());
        customstring.size = static_cast<uint32_t>(Level::created_dialoguemessages[Level::current_dialogue_id].content.length());
        return &customstring;
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x498CE4 && !Mission::created_missions.empty()) {
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.enabled) {
                static AEString customstring;
                customstring.text = const_cast<wchar_t*>(custom_mission.description.c_str());
                customstring.size = static_cast<uint32_t>(custom_mission.description.length());
                return &customstring;
            }
        }
    }
    // custom ships texts
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x45565e || reinterpret_cast<uintptr_t>(returnaddr) == 0x483296 || reinterpret_cast<uintptr_t>(returnaddr) == 0x4d99f8) {
        for (const auto& ship : Ship::created_ships) {
            if (id == ship.id + 892) { // + 892 bcz the game decided
                static AEString customstring;
                customstring.text = const_cast<wchar_t*>(ship.name.c_str());
                customstring.size = static_cast<uint32_t>(ship.name.length());
                return &customstring;
            }
        }
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x481bb8) {
        for (const auto& ship : Ship::created_ships) {
            if (id == ship.id + 953) { // + 953 bcz the game decided
                static AEString customstring;
                customstring.text = const_cast<wchar_t*>(ship.description.c_str());
                customstring.size = static_cast<uint32_t>(ship.description.length());
                return &customstring;
            }
        }
    }
    __asm {
        mov eax, id
        call old_gametextgettext
        mov result, eax
    }
    return result; // fun fact: this wasn't needed but I added it because it looked weird without, it worked due to of the return value being in eax
}

int __stdcall Hooks::recordhandler_recordstorewrite_hook(uintptr_t a, int b)
{
    // This function is useful to prevent custom items being injected in the save resulting in a crash at load...
    Globals_status** globals_status_ptr = reinterpret_cast<Globals_status**>(0x60AD6C);
    Globals_status* globals_status = *globals_status_ptr;
    int oldsize = 196;

    if (Item::created_items.size() == 0)
        return old_recordhandlerrecordstorewrite(a, b);
    if (globals_status->m_pItemLowestPrices && globals_status->m_pItemLowestPrices->size > oldsize) {
        AEArray<unsigned int>* oldarray = globals_status->m_pItemLowestPrices;
        AEArray<unsigned int>* newarray = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));

        newarray->size = oldsize;
        newarray->size2 = oldsize;
        newarray->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize));
        memcpy(newarray->data, oldarray->data, sizeof(unsigned int) * oldsize);
        globals_status->m_pItemLowestPrices = newarray;
        AbyssEngine::memory_free(oldarray->data);
        AbyssEngine::memory_free(oldarray);
    }
    if (globals_status->m_pItemHighestPrices && globals_status->m_pItemHighestPrices->size > oldsize) {
        AEArray<unsigned int>* oldarray = globals_status->m_pItemHighestPrices;
        AEArray<unsigned int>* newarray = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));

        newarray->size = oldsize;
        newarray->size2 = oldsize;
        newarray->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize));
        memcpy(newarray->data, oldarray->data, sizeof(unsigned int) * oldsize);
        globals_status->m_pItemHighestPrices = newarray;
        AbyssEngine::memory_free(oldarray->data);
        AbyssEngine::memory_free(oldarray);
    }
    if (globals_status->m_pItemLowestPricesSystem && globals_status->m_pItemLowestPricesSystem->size > oldsize) {
        AEArray<unsigned int>* oldarray = globals_status->m_pItemLowestPricesSystem;
        AEArray<unsigned int>* newarray = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));

        newarray->size = oldsize;
        newarray->size2 = oldsize;
        newarray->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize));
        memcpy(newarray->data, oldarray->data, sizeof(unsigned int) * oldsize);
        globals_status->m_pItemLowestPricesSystem = newarray;
        AbyssEngine::memory_free(oldarray->data);
        AbyssEngine::memory_free(oldarray);
    }
    if (globals_status->m_pItemHighestPricesSystem && globals_status->m_pItemHighestPricesSystem->size > oldsize) {
        AEArray<unsigned int>* oldarray = globals_status->m_pItemHighestPricesSystem;
        AEArray<unsigned int>* newarray = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));

        newarray->size = oldsize;
        newarray->size2 = oldsize;
        newarray->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize));
        memcpy(newarray->data, oldarray->data, sizeof(unsigned int) * oldsize);
        globals_status->m_pItemHighestPricesSystem = newarray;
        AbyssEngine::memory_free(oldarray->data);
        AbyssEngine::memory_free(oldarray);
    }

    int result = old_recordhandlerrecordstorewrite(a, b); // actual save
    Item::refreshitemsprices(); // restore array
    return result;
}

int __stdcall Hooks::level_creategun_hook(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    for (int i = 0; i < static_cast<int>(Item::created_items.size()); i++) {
        if (Item::created_items[i].item.m_nID == a2 && Item::created_items[i].type == "Laser") {
            a2 = 0;
        } else if (Item::created_items[i].item.m_nID == a2 && Item::created_items[i].type == "LaserBeam") {
            a2 = 10;
            a8 = Item::created_items[i].item.m_pItemInfo->data->m_nPropertyFourValue;
        } else if (Item::created_items[i].item.m_nID == a2 && Item::created_items[i].type == "Blaster") {
            a2 = 21;
        }
    }
    return old_levelcreategun(a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

int* __stdcall Hooks::level_createship_hook(uintptr_t *a1, int a2, int a3, int a4, uintptr_t *a5, int a6)
{
    //std::cout << a1 << std::endl;
    //std::cout << a2 << std::endl;
    //std::cout << a3 << std::endl;
    //std::cout << a4 << std::endl;
    //std::cout << a5 << std::endl;
    //std::cout << a6 << std::endl;
    //std::cout << "-----" << std::endl;
    // if not in a station then edit ships
    //if (a5 != 0) {
        //a2 = 6; // race
        //a4 = 40; // ship model
    //}
    return old_levelcreateship(a1, a2, a3, a4, a5, a6);
}

int __stdcall Hooks::imagefactory_drawchar_hook(int a2, int a3, int a4)
{
    unsigned int* a1;
    int returnvalue;
    void* returnaddr = nullptr;

    __asm {
        mov a1, ebx
        mov edx, [ebp + 4]
        mov returnaddr, edx
    }
    
    //AEArray<ImagePart*>* imageparts = reinterpret_cast<AEArray<ImagePart*>*>(a1);
    //std::cout << imageparts->size << std::endl;
    //std::cout << a1 << std::endl;
    //if (reinterpret_cast<uintptr_t>(returnaddr) == 0x4bccb3) {
    //    if (imageparts->data[0] == nullptr)
    //        return;
        //static int id1 = imageparts->data[0]->id - 1;
        //static int id2 = imageparts->data[1]->id - 1;
    //    static int id3 = imageparts->data[2]->id - 1;
        //static int id4 = imageparts->data[3]->id - 1;

        //imageparts->data[0]->id = id1;
        //imageparts->data[1]->id = id2;
    //    imageparts->data[2]->id = id3;
        //imageparts->data[3]->id = id4;
    //}
    //for (int i = 0; i < imageparts->size; i++) {
    //    if (imageparts->data[i] != nullptr)
    //        std::cout << "index " << i << " " << imageparts->data[i]->id << std::endl;
    //}
    __asm {
        push a4 
        push a3
        push a2

        mov ebx, a1
        call old_imagefactorydrawchar
        mov returnvalue, eax
    }
    return returnvalue;
}

void __stdcall Hooks::imagepart_draw_hook(unsigned int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    //std::cout << "a1 : " << a1 << std::endl;
    //std::cout << "a2 : " << a2 << std::endl;
    //std::cout << "a3 : " << a3 << std::endl;
    //std::cout << "a4 : " << a4 << std::endl;
    //std::cout << "a5 : " << a5 << std::endl;
    //std::cout << "a6 : " << a6 << std::endl;
    //std::cout << "a7 : " << a7 << std::endl;
    //std::cout << "a8 : " << a8 << std::endl;
   // a1 = 174;

    return old_imagepartdraw(a1, a2, a3, a4, a5, a6, a7, a8);
}

void __stdcall Hooks::abyssengine_paintcanvas_drawimage2d_hook(int a1, int a2, int a3, int a4, int a5)
{
    //if (reinterpret_cast<uintptr_t>(returnaddr) != 0x464D4D && reinterpret_cast<uintptr_t>(returnaddr) != 0x464F5D && reinterpret_cast<uintptr_t>(returnaddr) != 0x4DD46B && reinterpret_cast<uintptr_t>(returnaddr) != 0x464F25)
    //  std::cout << std::hex << returnaddr << std::endl;
    void* returnaddr = nullptr;

    __asm {
        mov edx, [ebp + 4]
        mov returnaddr, edx
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x45F5BC) {
        if (a2 == -1) {
            std::cout << a5 << std::endl;
            int a = Asset::image2dcreate(9872);
            return old_abyssenginepaintcanvasdrawimage2d(a1, a, a3, a4, a5);
        }
    }
    return old_abyssenginepaintcanvasdrawimage2d(a1, a2, a3, a4, a5);
}

int __stdcall Hooks::radiomessage_radiomessage_hook(int a4, int a5)
{
    int a1;
    int a2;
    int a3;
    int returnvalue;

    __asm {
        mov a1, eax
        mov a2, edi
        mov a3, esi
    }

    __asm {
        push a5
        push a4
        mov eax, a1
        mov edi, a2
        mov esi, a3
        call old_radiomessageradiomessage
        mov returnvalue, eax 
    }
    return returnvalue;
}

int __stdcall Hooks::imagefactory_loadchar_hook(int *a1)
{
    void* returnaddr = nullptr;

    __asm {
        mov edx, [ebp + 4]
        mov returnaddr, edx
    }
    // radio messages
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x04bca3c) {
        if (Level::created_radiomessages.empty())
            return old_imagefactoryloadchar(a1);

        int index = Level::created_radiomessages.size() == 1 ? 0 : Level::created_radiomessages.size() - 1;
        int* newimage = reinterpret_cast<int*>(AbyssEngine::memory_allocate(sizeof(int) * 5));
        newimage[0] = Level::created_radiomessages[index].race;
        newimage[1] = Level::created_radiomessages[index].hair;
        newimage[2] = Level::created_radiomessages[index].eyes;
        newimage[3] = Level::created_radiomessages[index].mouth;
        newimage[4] = Level::created_radiomessages[index].armor;
        int result = old_imagefactoryloadchar(newimage);
        AbyssEngine::memory_free(newimage);
        return result;
    }
    // dialogue window image
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x04075e3 && !Level::created_dialoguemessages.empty()) {
        Globals_appManager** globals_appmanager = reinterpret_cast<Globals_appManager**>(Offset::GLOBALS_APPMANAGER);
        MGame* mgame = reinterpret_cast<MGame*>((*globals_appmanager)->m_pCurrentModule);
        ModStation* mstation = reinterpret_cast<ModStation*>((*globals_appmanager)->m_pCurrentModule);
        mstation->m_pDialogueWindow->m_bFlipImage = 0;
        if (mgame->m_pLevel != nullptr && Level::created_dialoguemessages[Level::current_dialogue_id].isplayer)
            mgame->m_pDialogueWindow->m_bFlipImage = 1;
        else if (mstation->m_pDialogueWindow != nullptr && Level::created_dialoguemessages[Level::current_dialogue_id].isplayer)
            mstation->m_pDialogueWindow->m_bFlipImage = 1;
        int* newimage = reinterpret_cast<int*>(AbyssEngine::memory_allocate(sizeof(int) * 5));
        newimage[0] = Level::created_dialoguemessages[Level::current_dialogue_id].race;
        newimage[1] = Level::created_dialoguemessages[Level::current_dialogue_id].hair;
        newimage[2] = Level::created_dialoguemessages[Level::current_dialogue_id].eyes;
        newimage[3] = Level::created_dialoguemessages[Level::current_dialogue_id].mouth;
        newimage[4] = Level::created_dialoguemessages[Level::current_dialogue_id].armor;
        int result = old_imagefactoryloadchar(newimage);
        AbyssEngine::memory_free(newimage);
        return result;
    }
    return old_imagefactoryloadchar(a1);
}

int __stdcall Hooks::dialoguewindow_dialoguewindow_hook(int *a1, int *a2, int *a3, int a4)
{
    Level::current_dialogue_id = 0;
    return old_dialoguewindowdialoguewindow(a1, a2, a3, a4);
}

// TODO: I think I know why it didn't work (was missing &) lol
// Why am I using naked you may ask? because when i'm trying to hook this function normally like every others functions the game crash???
// Like it's a really weird crash and I spent 3 hours to figures out the crash and I couldn't figure it out... if you have a better way to do that then do a PR!!
static DWORD globals_dialoguesoundid = Offset::GLOBALS_GETDIALOGUESOUNDID;
static __declspec(naked) void globals_getdialoguesoundid_retpatch()
{
    __asm {
        call globals_dialoguesoundid
        mov eax, 0xffffffff
        ret
    }
}

int __stdcall Hooks::dialoguewindow_loadcontent_hook(int *a1)
{
    void* returnaddr = nullptr;
    __asm {
        mov edx, [ebp + 4]
        mov returnaddr, edx
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x406456)
        if (!Level::created_dialoguemessages.empty() && Level::current_dialogue_id < static_cast<int>(Level::created_dialoguemessages.size()) - 1)
            Level::current_dialogue_id++;
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x4078eb)
        if (!Level::created_dialoguemessages.empty() && Level::current_dialogue_id > 0)
            Level::current_dialogue_id--;
    if (!Level::created_dialoguemessages.empty()) {
        DWORD callsite = 0x407617;
        DWORD target = reinterpret_cast<DWORD>(&globals_getdialoguesoundid_retpatch);
        DWORD relativeoffset = target - (callsite + 5);
        DWORD old;
        VirtualProtect(reinterpret_cast<void*>(callsite), 5, PAGE_EXECUTE_READWRITE, &old);
        *reinterpret_cast<BYTE*>(callsite) = 0xe8;
        *reinterpret_cast<DWORD*>((callsite + 1)) = relativeoffset;
        VirtualProtect(reinterpret_cast<void*>(callsite), 5, old, &old);
    }
    return old_dialoguewindowloadcontent(a1);
}

int __stdcall Hooks::dialoguewindow_set_hook(int a4)
{
    int returnvalue;
    int *a1;
    int a2;
    int *a3;

    __asm {
        mov a1, eax
        mov a2, edx
        mov a3, ecx
    }

    __asm {
        mov eax, a1
        mov edx, a2
        mov ecx, a3
        push a4
        call old_dialoguewindowset
        mov returnvalue, eax
    }
    return returnvalue;
}

int __cdecl Hooks::mgame_togglepause_hook()
{
    int* a1 = nullptr;
    int returnvalue = 0;
    char a2 = 0;

    __asm {
        mov a1, eax
        mov a2, cl
    }
    __asm {
        mov eax, a1
        movzx ecx, a2
        mov edx, old_mgametogglepause
        call edx
        mov returnvalue, eax        
    }
    return returnvalue; 
}

int __cdecl Hooks::cutscene_cutscene_hook()
{
    int a1 = 0;
    int* a2 = nullptr;
    int returnvalue = 0;

    __asm {
        mov a2, eax
        mov a1, ecx
    }
    std::cout << "a1: " << a1 << std::endl;
    std::cout << "a2: " << a2 << std::endl;
    __asm {
        mov eax, a2
        mov ecx, a1
        mov edx, old_cutscenecutscene
        call edx
        mov returnvalue, eax
    }
    return returnvalue;
}

float* __cdecl Hooks::targetfollowcamera_setposition_hook()
{
    int* a1 = nullptr;
    float* a2 = nullptr;
    float* returnvalue = nullptr;

    __asm {
        mov a1, eax
        mov a2, ecx
    }

    std::cout << "Targetfollowcamera: " << a1 << std::endl;
    std::cout << "Vector: " << a2 << std::endl;

    __asm {
        mov eax, a1
        mov ecx, a2
        mov edx, old_targetfollowcamerasetposition
        call edx
        mov returnvalue, eax
    }

    return returnvalue;
}

char __thiscall Hooks::levelscript_process_hook(LevelScript* a1, int a2)
{
    if (!Level::created_cutscene)
        return old_levelscriptprocess(a1, a2);
    if (Level::created_cutscenepts.empty()) {
        Level::created_cutscene = 0;
        return old_levelscriptprocess(a1, a2);
    }
    TargetFollowCamera* camera = a1->m_pCamera;
    if (a1->m_nState == 0) {
        a1->m_nState = 1000; // 1000 so we are sure it doesn't exist in the game
        Level::cutscene_timer = 0.0f;
        Level::cutscene_pointid = 0;
        Level::cutscene_anchored = false;
        if (camera != nullptr) // just to be sure
            Level::cutscene_originaltarget = reinterpret_cast<uintptr_t>(camera->m_pTarget);
        return 1;
    }
    if (a1->m_nState >= 1000) {
        if (camera == nullptr)
            return 1;
        int points = static_cast<int>(Level::created_cutscenepts.size());
        if (Level::cutscene_pointid >= points) {
            if (Level::cutscene_originaltarget != 0)
                camera->m_pTarget = reinterpret_cast<void*>(Level::cutscene_originaltarget);
            camera->m_nShakeAmount = 0.0f;
            camera->m_nShakeFrequency = 0;
            Level::created_cutscene = 0;
            Level::created_cutscenepts.clear();
            Level::cutscene_timer = 0.0f;
            Level::cutscene_pointid = 0;
            a1->m_nState = 0;
            return 1;
        }
        if (!Level::cutscene_anchored) {
            if (camera->m_nPosX != 0.0f && !std::isnan(camera->m_nPosX) && std::abs(camera->m_nPosX) > 1.0f) {
                Level::cutscene_anchorX = camera->m_nPosX;
                Level::cutscene_anchorY = camera->m_nPosY;
                Level::cutscene_anchorZ = camera->m_nPosZ;
                Level::cutscene_anchored = true;
            }
            return 1;
        }
        Level::cutscene_timer += static_cast<float>(a2); // dt
        camera->m_nPosX = Level::cutscene_anchorX + Level::created_cutscenepts[Level::cutscene_pointid].x;
        camera->m_nPosY = Level::cutscene_anchorY + Level::created_cutscenepts[Level::cutscene_pointid].y;
        camera->m_nPosZ = Level::cutscene_anchorZ + Level::created_cutscenepts[Level::cutscene_pointid].z;
        camera->m_nShakeAmount = Level::created_cutscenepts[Level::cutscene_pointid].shakeamount;
        camera->m_nShakeFrequency = Level::created_cutscenepts[Level::cutscene_pointid].shakefrequency;
        if (Level::cutscene_timer >= Level::created_cutscenepts[Level::cutscene_pointid].duration) {
            camera->m_nShakeAmount = 0.0f;
            camera->m_nShakeFrequency = 0;
            Level::cutscene_pointid = Level::cutscene_pointid + 1;
            Level::cutscene_timer = 0.0f;
        }
        return 1;
    }
    return old_levelscriptprocess(a1, a2);
}

int __stdcall Hooks::route_route_hook(int a2, int a3)
{
    int* a1;
    int returnvalue;
    
    __asm {
        mov a1, ebx
    }
    std::cout << a1 << std::endl;
    std::cout << a2 << std::endl;
    std::cout << a3 << std::endl;
    //a3 = 0; a3 how much waypoints left?
    __asm {
        push a3
        push a2

        mov ebx, a1
        call old_routeroute
        mov returnvalue, eax
    }
    return returnvalue;
}

int __stdcall Hooks::waypoint_waypoint_hook(int a2, int a3, int a4, int* a5)
{
    int* a1;
    int returnvalue;
    void* returnaddr = nullptr;

    __asm {
        mov a1, esi
        mov edx, [ebp + 4]
        mov returnaddr, edx
    }
    std::cout << "Waypoint: " << a1 << std::endl;
    std::cout << "a2: " << a2 << std::endl;
    std::cout << "a3: " << a3 << std::endl;
    std::cout << "a4: " << a4 << std::endl;
    std::cout << "Route: " << a5 << std::endl;
    std::cout << "returnaddr: " << returnaddr << std::endl;
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x004C23A0) {
        //    a2 = 100000;
        //a3 = 100000;
        //a4 = 100000;
    }
    __asm {
        push a5
        push a4
        push a3
        push a2

        mov esi, a1
        call old_waypointwaypoint
        mov returnvalue, eax
    }
    return returnvalue;
}

void __stdcall Hooks::radar_draw_hook(int* a1, float* a2, int* a3, int a4)
{
    std::cout << "radar: " << a1 << std::endl;
    std::cout << "player: " << a2 << std::endl;
    std::cout << "hud: " << a3 << std::endl;
    return old_radardraw(a1, a2, a3, a4);
}

int __cdecl Hooks::route_getwaypoint_hook()
{
    int* a1 = nullptr;
    int returnvalue = 0;

    __asm {
        mov a1, eax
    }
    std::cout << "a1: " << a1 << std::endl;
    __asm {
        mov eax, a1
        mov edx, old_routegetwaypoint
        call edx
        mov returnvalue, eax
    }
    return returnvalue;
}

float* __thiscall Hooks::playerfighter_render_hook(unsigned int *a1)
{
    return old_playerfighterrender(a1);
}

int* __stdcall Hooks::level_assignguns_hook(int *a1)
{
    int* returnv = old_levelassignguns(a1);
    std::cout << "assign gun ptr: " << a1 << std::endl;
    std::cout << "returnval assigngun : " << returnv << std::endl;
    return returnv;
}

/*int* __stdcall Hooks::level_assignguns_hook(int *a1)
{
    int* returnvalue = old_levelassignguns(a1);

    if (a1[48] != 0) {
        int* gunarray = reinterpret_cast<int*>(a1[48]);
        gunarray[0] = 0;
    }
    return returnvalue;
}*/

void __stdcall Hooks::kiplayer_addgun_hook(int a3)
{
    int** a1;
    int* a2;

    __asm {
        mov a1, ebx
        mov a2, edi
    }
    std::cout << "===ADDGUN===" << std::endl;
    std::cout << std::hex << a1 << std::endl;
    std::cout << std::hex << a2 << std::endl;
    std::cout << a3 << std::endl;
    std::cout << "===end ADDGUN==" << std::endl;
    __asm {
        mov ebx, a1
        mov edi, a2
    }
    old_kiplayeraddgun(a3);
}

int* __stdcall Hooks::globals_getshipgroup_hook(int a1, int a2, char a3)
{
    std::cout << std::dec << a1 << std::endl;
    return old_globalsgetshipgroup(a1, a2, a3);
}

void __stdcall Hooks::level_update_hook(int a2, int a3, int a4)
{
    int* a1;
    __asm {
        mov a1, eax
    }
    std::cout << std::hex << a1 << std::endl;
    std::cout << a2 << std::endl;
    std::cout << a3 << std::endl;
    std::cout << a4 << std::endl;
    __asm {
        mov eax, a1
        push a4
        push a3
        push a2
        call old_levelupdate
    }
}

// TODO: bro wtf my function just looks so bad i'll def rewrite it lmao, same for createcampaignmission
void __thiscall Hooks::level_createmission_hook(void *a1)
{
    MissionContext ctx = EventManager::trigger_hook<MissionContext>("Level::createMission");
    int* level = reinterpret_cast<int*>(a1);
    uintptr_t address_createship = Offset::LEVEL_CREATESHIP;
    uintptr_t address_createstaticobject = Offset::LEVEL_CREATESTATICOBJECT;
    uintptr_t address_setposition = Offset::PLAYERFIXEDOBJECT_SETPOSITION;
    std::vector<int*> spawnednpcs;

    for (const auto& fighter : Level::created_playerfighters) {
        int shiptype = fighter.meshid;
        int faction = fighter.faction;
        int* plrfighterptr = nullptr;
        __asm {
            push 1
            push 0
            push shiptype
            push 0
            push faction
            push level
            call address_createship
            mov plrfighterptr, eax
        }
        uintptr_t address_assignguns = Offset::LEVEL_ASSIGNGUNS;
        __asm {
            push level
            call address_assignguns
        }
        spawnednpcs.push_back(plrfighterptr);
    }
    for (const auto& object : Level::created_staticobjects) {
        int objtype = 0;
        if (object.type == 0)
            objtype = 14243; // pirate station
        else if (object.type == 1)
            objtype = 14363; // valkyrie turret
        int* levelptr = level;
        int* objectptr = nullptr;
        __asm {
            push objtype
            push 0
            push levelptr
            call address_createstaticobject        
            mov objectptr, eax
        }
        typedef void (__thiscall* SetPositionFn)(void* thisptr, float x, float y, float z);
        float x = static_cast<float>(object.x); // even if it's already a float I still need to cast it to a float or else the game will crash
        float y = static_cast<float>(object.y);
        float z = static_cast<float>(object.z);
        //uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(objectptr);
        if (objtype == 14243) { // TODO: only pirate base support coordinate editing atm (game crash if it's the turret)
            __asm {
                push z
                push y
                push x
                mov ecx, objectptr
                call address_setposition
            }
        }
        spawnednpcs.push_back(objectptr);
    }
    Level::created_playerfighters.clear(); // maybe use ctx for args instead of vectors?
    Level::created_staticobjects.clear();
    if (ctx.call_original)
        old_levelcreatemission(a1);
    if (!spawnednpcs.empty()) {
        AEArray<int*>* entitylist = reinterpret_cast<SingleLevel*>(a1)->m_pEntities;
        if (entitylist) {
            int oldcount = entitylist->size;
            int newcount = oldcount + static_cast<uint32_t>(spawnednpcs.size());
            int** newdata = reinterpret_cast<int**>(AbyssEngine::memory_allocate(newcount * sizeof(int*)));
            if (newdata) {
                if (entitylist->data && oldcount > 0)
                    memcpy(newdata, entitylist->data, oldcount * sizeof(int*));
                for (int i = 0; i < static_cast<int>(spawnednpcs.size()); i++)
                    newdata[oldcount + i] = spawnednpcs[i];
                entitylist->data = newdata;
                entitylist->size = newcount;
                entitylist->size2 = newcount;
            }
        }
    }
}

int __thiscall Hooks::level_createcampaignmission_hook(void *a1)
{
    MissionContext ctx = EventManager::trigger_hook<MissionContext>("Level::createCampaignMission");
    int* level = reinterpret_cast<int*>(a1);
    uintptr_t address_createship = Offset::LEVEL_CREATESHIP;
    uintptr_t address_createstaticobject = Offset::LEVEL_CREATESTATICOBJECT;
    uintptr_t address_setposition = Offset::PLAYERFIXEDOBJECT_SETPOSITION;
    int result = 0;
    std::vector<int*> spawnednpcs;

    for (const auto& fighter : Level::created_playerfighters) {
        int shiptype = fighter.meshid;
        int faction = fighter.faction;
        int* plrfighterptr = nullptr;
        __asm {
            push 1
            push 0
            push shiptype
            push 0
            push faction
            push level
            call address_createship
            mov plrfighterptr, eax
        }
        uintptr_t address_assignguns = Offset::LEVEL_ASSIGNGUNS;
        __asm {
            push level
            call address_assignguns
        }
        spawnednpcs.push_back(plrfighterptr);
    }
    for (const auto& object : Level::created_staticobjects) {
        int objtype = 0;
        if (object.type == 0)
            objtype = 14243; // pirate station
        else if (object.type == 1)
            objtype = 14363; // valkyrie turret
        int* levelptr = level;
        int* objectptr = nullptr;
        __asm {
            push objtype
            push 0
            push levelptr
            call address_createstaticobject        
            mov objectptr, eax
        }
        typedef void (__thiscall* SetPositionFn)(void* thisptr, float x, float y, float z);
        float x = static_cast<float>(object.x); // even if it's already a float I still need to cast it to a float or else the game will crash
        float y = static_cast<float>(object.y);
        float z = static_cast<float>(object.z);
        //uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(objectptr);
        if (objtype == 14243) { // TODO: only pirate base support coordinate editing atm (game crash if it's the turret)
            __asm {
                push z
                push y
                push x
                mov ecx, objectptr
                call address_setposition
            }
        }
        spawnednpcs.push_back(objectptr);
    }
    Level::created_playerfighters.clear(); // maybe use ctx for args instead of vectors?
    Level::created_staticobjects.clear();
    if (ctx.call_original)
        result = old_levelcreatecampaignmission(a1);
    if (!spawnednpcs.empty()) {
        AEArray<int*>* entitylist = reinterpret_cast<SingleLevel*>(a1)->m_pEntities;
        if (!entitylist) { // it's already null but just to make sure lmao
            entitylist = reinterpret_cast<AEArray<int*>*>(AbyssEngine::memory_allocate(sizeof(AEArray<int*>)));
            if (entitylist) {
                entitylist->size = 0;
                entitylist->data = nullptr;
                entitylist->size2 = 0;                
                reinterpret_cast<SingleLevel*>(a1)->m_pEntities = entitylist; 
            }
        }
        if (entitylist) {
            int oldcount = entitylist->size;
            int newcount = oldcount + static_cast<uint32_t>(spawnednpcs.size());
            int** newdata = reinterpret_cast<int**>(AbyssEngine::memory_allocate(newcount * sizeof(int*)));
            if (newdata) {
                if (entitylist->data && oldcount > 0)
                    memcpy(newdata, entitylist->data, oldcount * sizeof(int*));
                for (int i = 0; i < static_cast<int>(spawnednpcs.size()); i++)
                    newdata[oldcount + i] = spawnednpcs[i];
                entitylist->data = newdata;
                entitylist->size = newcount;
                entitylist->size2 = newcount;
            }
        }
    }
    return result;
}

void __thiscall Hooks::modmainmenu_onrender2d_hook(int *a1)
{
    Render2DContext ctx = EventManager::trigger_hook<Render2DContext>("ModMainMenu::OnRender2D");

    if (ctx.call_original)
        old_modmainmenuonrender2d(a1);
}

int* __stdcall Hooks::level_createstaticobject_hook(int a1, int *a2, int a3)
{
    std::cout << "hiii" << std::endl;
    return old_levelcreatestaticobject(a1, a2, a3);
}

void __thiscall Hooks::mgame_onrender2d_hook(int *a1)
{
    Render2DContext ctx = EventManager::trigger_hook<Render2DContext>("MGame::OnRender2D");

    if (ctx.call_original)
        old_mgameonrender2d(a1);
}

unsigned int __stdcall Hooks::level_createspace_hook(int *a1)
{
    return old_levelcreatespace(a1);
}

void __thiscall Hooks::status_nextcampaignmission_hook(int *a1)
{
    old_statusnextcampaignmission(a1);
}

int __fastcall Hooks::starmap_init_hook(int a1, int a2, char a3, int a4, char a5)
{
    Globals_status** status_ptr = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
    Globals_status* status = *status_ptr;
    
    // if they do alt f4 in the starmap the save will just be corrupted so yeah good luck w/ that it's a user issue to me
    if (status && status->m_pMission) {
        SingleMission* m = reinterpret_cast<SingleMission*>(status->m_pMission);
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.enabled) {
                m->m_nMissionEnabled = 1;
                m->m_nStationId = custom_mission.stationid;
                break; 
            }
        }
    }
    return old_starmapinit(a1, a2, a3, a4, a5);
}

int __stdcall Hooks::starmap_starmap_hook(int a2, int a3, int a4, int a5)
{
    int a1;
    int returnvalue;
    
    __asm {
        mov a1, ebx
    }
    __asm {
        push a5
        push a4
        push a3
        push a2
        mov ebx, a1
        call old_starmapstarmap
        mov returnvalue, eax
    }
    return returnvalue;
}

int __cdecl Hooks::status_getcampaignmission_hook()
{
    int* a1;
    int returnvalue;
    void* returnaddr;

    __asm {
        mov a1, eax      
        mov eax, [ebp + 4]   
        mov returnaddr, eax
        pushad
        pushfd
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x4D044A && Mission::created_missions.size() > 0) {
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.enabled && custom_mission.type == 1) {
                __asm {
                    popfd
                    popad
                }
                return 0;
            }
        }
    }
    __asm {
        popfd
        popad
        mov eax, a1   
        call old_statusgetcampaignmission
        mov returnvalue, eax
    }
    return returnvalue;
}

int __cdecl Hooks::status_getfreelancemission_hook()
{
    // TODO: what if we have a real campaign mission? then for the getcampaignmission what if we have a real freelance mission? make your own array with every ongoing missions (hook the accept mission idk)
    int* a1;
    int returnvalue;
    void* returnaddr = nullptr;

    __asm {
        mov a1, eax
        mov eax, [ebp+4]
        mov returnaddr, eax
        pushad
        pushfd
    }
    __asm {
        popfd
        popad
    }
    if (reinterpret_cast<uintptr_t>(returnaddr) == 0x4D0452 && Mission::created_missions.size() > 0) {
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.enabled && custom_mission.type == 0) {
                __asm {
                    mov eax, 0
                }
                return 0;
            }
        }
    }
    __asm {
        mov eax, a1
        call old_statusgetfreelancemission
        mov returnvalue, eax
    }
    return returnvalue;
}

int __cdecl Hooks::starmap_dtor_hook()
{
    int *a1;
    int returnvalue;

    __asm {
        mov a1, edi
    }
    std::cout << "StarMap::~StarMap()" << std::endl;
    __asm {
        mov edi, a1
        call old_starmapdtor
        mov returnvalue, eax
    }
    return returnvalue;
}

char __fastcall Hooks::starmap_ontouchend_hook(int a1, int *a2, int a3)
{
    std::cout << *(int*)((uintptr_t)a2 + 0x70) << std::endl;
    std::cout << std::hex << a2 << std::endl;
    return old_starmaptouchend(a1, a2, a3);
}

char __cdecl Hooks::modstation_leavestation_hook()
{
    int a1;
    char returnvalue;

    __asm {
        mov a1, esi
    }
    __asm {
        mov esi, a1
        call old_modstationleavestation
        mov returnvalue, al
    }
    return returnvalue;
}

int __thiscall Hooks::modstation_onupdate_hook(ModStation *a1)
{
    if (!a1->m_nStarMapWindowOpen && Mission::created_missions.size() > 0) {
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.enabled) {
                Globals_status** status_ptr = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
                Globals_status* status = *status_ptr;
                SingleMission *m = reinterpret_cast<SingleMission*>(status->m_pMission);
                m->m_nMissionEnabled = -1;
                m->m_nStationId = 0;
                break; 
            }
        }
    }
    return old_modstationonupdate(a1);
}

void __thiscall Hooks::mgame_onupdate_hook(MGame *a1)
{
    // this is needed because if we create a custom mission and then click on "close" or "skip" in a dialogue window then the real mission id will be incremented lol
    if (Mission::created_missions.size() > 0 && *(bool*)((uintptr_t)a1 + 0x5e)) {
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.entered_mission) {
                Globals_status** status_ptr = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
                Globals_status* status = *status_ptr;
                Mission::savedrealmissionid = status->m_nCurrentCampaignMission;
            }
        }
    }
    if (Mission::created_missions.size() > 0 && !*(bool*)((uintptr_t)a1 + 0x5e)) {
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.entered_mission && Mission::savedrealmissionid != 0) {
                Globals_status** status_ptr = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
                Globals_status* status = *status_ptr;
                status->m_nCurrentCampaignMission = Mission::savedrealmissionid;
                Mission::savedrealmissionid = 0;
            }
        }
    }
    // Prevents "Missions" button crashing the game
    if (!a1->m_nStarMapWindowOpen && Mission::created_missions.size() > 0) {
        for (const auto& custom_mission : Mission::created_missions) {
            if (custom_mission.enabled && custom_mission.entered_mission) {
                Globals_status** status_ptr = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
                Globals_status* status = *status_ptr;
                SingleMission *m = reinterpret_cast<SingleMission*>(status->m_pMission);
                m->m_nMissionEnabled = 1;
                m->m_nStationId = custom_mission.stationid;
            }
            if (custom_mission.enabled && !custom_mission.entered_mission) {
                Globals_status** status_ptr = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
                Globals_status* status = *status_ptr;
                SingleMission *m = reinterpret_cast<SingleMission*>(status->m_pMission);
                m->m_nMissionEnabled = -1;
                m->m_nStationId = 0;
                break; 
            }
        }
    }
    old_mgameonupdate(a1);
}

// TODO: executed only by ModStation...
int* __thiscall Hooks::dialoguewindow_dtor_hook(void *a1, int *a2)
{
    if (!Level::created_dialoguemessages.empty())
        Level::created_dialoguemessages.clear();
    return old_dialoguewindowdtor(a1, a2);
}

int __stdcall Hooks::starmap_depart_hook(int a2)
{
    int a1;
    int returnvalue;

    __asm {
        mov a1, eax
    }
    for (const auto& custom_mission : Mission::created_missions) {
        if (custom_mission.enabled && !custom_mission.entered_mission) {
            Globals_status** status_ptr = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
            Globals_status* status = *status_ptr;
            SingleMission *m = reinterpret_cast<SingleMission*>(status->m_pMission);
            m->m_nMissionEnabled = -1;
            m->m_nStationId = 0;
            break; 
        }
    }
    __asm {
        push a2
        mov eax, a1
        call old_starmapdepart
        mov returnvalue, eax
    }
    return returnvalue;
}

float* __stdcall Hooks::playerego_calccollision_hook(int a1, float *a2)
{
    return old_playeregocalccollision(a1, a2);
}

unsigned int* __stdcall Hooks::fileread_loadweaponpositions_hook(int a1)
{
    if (a1 < 44)
        return old_filereadloadweaponpositions(a1);
    // loading primary pos, secondary pos, turret pos and finally the engine pos
    auto& ship = Ship::created_ships[a1 - 44];
    auto* result = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(12));
    auto* arr = reinterpret_cast<DWORD*>(AbyssEngine::memory_allocate(4 * sizeof(DWORD)));
    std::memset(arr, 0, 4 * sizeof(DWORD));
    result[0] = 4;
    result[1] = reinterpret_cast<unsigned int>(arr);
    result[2] = 4;
    int id = 0;
    int primary_size = static_cast<int>(ship.primary_positions.size());
    int primary_cap = primary_size > 0 ? primary_size : 1; // game crash if 0
    auto* primary_arr = reinterpret_cast<int*>(AbyssEngine::memory_allocate(12));
    auto* primary_data = reinterpret_cast<int*>(AbyssEngine::memory_allocate(primary_cap * sizeof(int)));
    std::memset(primary_data, 0, primary_cap * sizeof(int));
    primary_arr[0] = primary_size;
    primary_arr[1] = reinterpret_cast<int>(primary_data);
    primary_arr[2] = primary_cap;
    arr[0] = reinterpret_cast<DWORD>(primary_arr);
    id = 0;
    for (auto& pos : ship.primary_positions) {
        auto* entry = reinterpret_cast<float*>(AbyssEngine::memory_allocate(12));
        entry[0] = pos.x;
        entry[1] = pos.y;
        entry[2] = pos.z;
        primary_data[id++] = reinterpret_cast<int>(entry);
    }
    int secondary_size = static_cast<int>(ship.secondary_positions.size());
    int secondary_cap = secondary_size > 0 ? secondary_size : 1; // same as primary
    auto* secondary_arr = reinterpret_cast<int*>(AbyssEngine::memory_allocate(12));
    auto* secondary_data = reinterpret_cast<int*>(AbyssEngine::memory_allocate(secondary_cap * sizeof(int)));
    std::memset(secondary_data, 0, secondary_cap * sizeof(int));
    secondary_arr[0] = secondary_size;
    secondary_arr[1] = reinterpret_cast<int>(secondary_data);
    secondary_arr[2] = secondary_cap;
    arr[1] = reinterpret_cast<DWORD>(secondary_arr);
    id = 0;
    for (auto& pos : ship.secondary_positions) {
        auto* entry = reinterpret_cast<float*>(AbyssEngine::memory_allocate(12));
        entry[0] = pos.x;
        entry[1] = pos.y;
        entry[2] = pos.z;
        secondary_data[id++] = reinterpret_cast<int>(entry);
    }
    int turret_size = static_cast<int>(ship.turret_positions.size());
    int turret_cap = turret_size > 0 ? turret_size : 1; // same as primary and secondary
    auto* turret_arr = reinterpret_cast<int*>(AbyssEngine::memory_allocate(12));
    auto* turret_data = reinterpret_cast<int*>(AbyssEngine::memory_allocate(turret_cap * sizeof(int)));
    std::memset(turret_data, 0, turret_cap * sizeof(int));
    turret_arr[0] = turret_size;
    turret_arr[1] = reinterpret_cast<int>(turret_data);
    turret_arr[2] = turret_cap;
    arr[2] = reinterpret_cast<DWORD>(turret_arr);
    id = 0;
    for (auto& pos : ship.turret_positions) {
        auto* entry = reinterpret_cast<float*>(AbyssEngine::memory_allocate(12));
        entry[0] = pos.x;
        entry[1] = pos.y;
        entry[2] = pos.z;
        turret_data[id++] = reinterpret_cast<int>(entry);
    }
    int engine_size = static_cast<int>(ship.engines.size() * 2);
    int engine_cap = engine_size > 0 ? engine_size : 1;
    auto* engine_arr = reinterpret_cast<int*>(AbyssEngine::memory_allocate(12));
    auto* engine_data = reinterpret_cast<int*>(AbyssEngine::memory_allocate(engine_cap * sizeof(int)));
    std::memset(engine_data, 0, engine_cap * sizeof(int));
    engine_arr[0] = engine_size;
    engine_arr[1] = reinterpret_cast<int>(engine_data);
    engine_arr[2] = engine_cap;
    arr[3] = reinterpret_cast<DWORD>(engine_arr);
    id = 0;
    for (auto& engine : ship.engines) {
        auto* pos_entry = reinterpret_cast<float*>(AbyssEngine::memory_allocate(12));
        pos_entry[0] = engine.position.x;
        pos_entry[1] = engine.position.y;
        pos_entry[2] = engine.position.z;
        engine_data[id++] = reinterpret_cast<int>(pos_entry);
        auto* intensity_entry = reinterpret_cast<float*>(AbyssEngine::memory_allocate(12));
        intensity_entry[0] = engine.intensity.x;
        intensity_entry[1] = engine.intensity.y;
        intensity_entry[2] = engine.intensity.z;
        engine_data[id++] = reinterpret_cast<int>(intensity_entry);
    }
    return result;
}

void __stdcall Hooks::imagefactory_drawitem_hook(int a3, unsigned int a4)
{
    int a1;
    int a2;

    __asm {
        mov a1, ebx
        mov a2, edi
    }
    if (a3 >= 196) {
        for (const auto& item : Item::created_items) {
            if (a3 == item.id)
                a3 = item.spriteicon - 3824; // 3824 bcz the game decided
        }
    }
    __asm {
        push a4
        push a3
        mov ebx, a1
        mov edi, a2
        call old_imagefactorydrawitem
    }
}

void Hooks::init()
{
    MH_Initialize();
    MH_CreateHook((LPVOID)Offset::GLOBALS_INIT_ADDR, &globals_init_hook, (LPVOID*)&oldglobals_init);
    MH_CreateHook((LPVOID)Offset::FILEREAD_LOADSTATIONBINARYFROMID, &fileread_loadstationbinaryfromid_hook, (LPVOID*)&old_filereadloadstationbinaryfromid);
    MH_CreateHook((LPVOID)Offset::FILEREAD_LOADSTATIONBIRARY, &fileread_loadstationbinary_hook, (LPVOID*)&old_filereadloadstationbinary);
    //MH_CreateHook((LPVOID)Offset::STANDING_ISENEMY, &standing_isenemy_hook, (LPVOID*)&old_standingisenemy);
    //MH_CreateHook((LPVOID)Offset::ABYSSENGINE_PAINTCANVAS_SETCOLOR, &abyssengine_paintcanvas_setcolor_hook, (LPVOID*)&old_abyssenginepaintcanvassetcolor);
    MH_CreateHook((LPVOID)Offset::GAMETEXT_GETTEXT, &gametext_gettext_hook, (LPVOID*)&old_gametextgettext);
    MH_CreateHook((LPVOID)Offset::RECORDHANDLER_RECORDSTOREWRITE, &recordhandler_recordstorewrite_hook, (LPVOID*)&old_recordhandlerrecordstorewrite);
    MH_CreateHook((LPVOID)Offset::LEVEL_CREATEGUN, &level_creategun_hook, (LPVOID*)&old_levelcreategun);
    //MH_CreateHook((LPVOID)Offset::LEVEL_CREATESHIP, &level_createship_hook, (LPVOID*)&old_levelcreateship);
    //MH_CreateHook((LPVOID)Offset::IMAGEFACTORY_DRAWCHAR, &imagefactory_drawchar_hook, (LPVOID*)&old_imagefactorydrawchar);
    //MH_CreateHook((LPVOID)Offset::IMAGEPART_DRAW, &imagepart_draw_hook, (LPVOID*)&old_imagepartdraw);
    //MH_CreateHook((LPVOID)Offset::ABYSSENGINE_PAINTCANVAS_DRAWIMAGE2D, &abyssengine_paintcanvas_drawimage2d_hook, (LPVOID*)&old_abyssenginepaintcanvasdrawimage2d);
    //MH_CreateHook((LPVOID)Offset::RADIOMESSAGE_RADIOMESSAGE, &radiomessage_radiomessage_hook, (LPVOID*)&old_radiomessageradiomessage);
    MH_CreateHook((LPVOID)Offset::IMAGEFACTORY_LOADCHAR, &imagefactory_loadchar_hook, (LPVOID*)&old_imagefactoryloadchar);
    MH_CreateHook((LPVOID)Offset::DIALOGUEWINDOW_DIALOGUEWINDOW, &dialoguewindow_dialoguewindow_hook, (LPVOID*)&old_dialoguewindowdialoguewindow);
    MH_CreateHook((LPVOID)Offset::DIALOGUEWINDOW_LOADCONTENT, &dialoguewindow_loadcontent_hook, (LPVOID*)&old_dialoguewindowloadcontent);
    //MH_CreateHook((LPVOID)Offset::DIALOGUEWINDOW_SET, &dialoguewindow_set_hook, (LPVOID*)&old_dialoguewindowset);
    //MH_CreateHook((LPVOID)Offset::MGAME_TOGGLEPAUSE, &mgame_togglepause_hook, (LPVOID*)&old_mgametogglepause);
    //MH_CreateHook((LPVOID)Offset::CUTSCENE_CUTSCENE, &cutscene_cutscene_hook, (LPVOID*)&old_cutscenecutscene);
    //MH_CreateHook((LPVOID)Offset::TARGETFOLLOWCAMERA_SETPOSITION, &targetfollowcamera_setposition_hook, (LPVOID*)&old_targetfollowcamerasetposition);
    MH_CreateHook((LPVOID)Offset::LEVELSCRIPT_PROCESS, &levelscript_process_hook, (LPVOID*)&old_levelscriptprocess);
    //MH_CreateHook((LPVOID)Offset::ROUTE_ROUTE, &route_route_hook, (LPVOID*)&old_routeroute);
    //MH_CreateHook((LPVOID)Offset::WAYPOINT_WAYPOINT, &waypoint_waypoint_hook, (LPVOID*)&old_waypointwaypoint);
    //MH_CreateHook((LPVOID)Offset::RADAR_DRAW, &radar_draw_hook, (LPVOID*)&old_radardraw);
    //MH_CreateHook((LPVOID)Offset::ROUTE_GETWAYPOINT, (LPVOID)&route_getwaypoint_hook, (LPVOID*)&old_routegetwaypoint);
    //MH_CreateHook((LPVOID)Offset::PLAYERFIGHTER_RENDER, (LPVOID)&playerfighter_render_hook, (LPVOID*)&old_playerfighterrender);
    //MH_CreateHook((LPVOID)Offset::LEVEL_ASSIGNGUNS, (LPVOID)&level_assignguns_hook, (LPVOID*)&old_levelassignguns);
    //MH_CreateHook((LPVOID)Offset::KIPLAYER_ADDGUN, (LPVOID)&kiplayer_addgun_hook, (LPVOID*)&old_kiplayeraddgun);
    //MH_CreateHook((LPVOID)Offset::GLOBALS_GETSHIPGROUP, (LPVOID)&globals_getshipgroup_hook, (LPVOID*)&old_globalsgetshipgroup);
    //MH_CreateHook((LPVOID)Offset::LEVEL_UPDATE, (LPVOID)&level_update_hook, (LPVOID*)&old_levelupdate);
    MH_CreateHook((LPVOID)Offset::LEVEL_CREATEMISSION, (LPVOID)&level_createmission_hook, (LPVOID*)&old_levelcreatemission);
    MH_CreateHook((LPVOID)Offset::MODMAINMENU_ONRENDER2D, (LPVOID)&modmainmenu_onrender2d_hook, (LPVOID*)&old_modmainmenuonrender2d);
    //MH_CreateHook((LPVOID)Offset::LEVEL_CREATESTATICOBJECT, (LPVOID)&level_createstaticobject_hook, (LPVOID*)&old_levelcreatestaticobject);
    MH_CreateHook((LPVOID)Offset::MGAME_ONRENDER2D, (LPVOID)&mgame_onrender2d_hook, (LPVOID*)&old_mgameonrender2d);
    //MH_CreateHook((LPVOID)Offset::LEVEL_CREATESPACE, (LPVOID)&level_createspace_hook, (LPVOID*)&old_levelcreatespace);
    //MH_CreateHook((LPVOID)Offset::STATUS_NEXTCAMPAIGNMISSION, (LPVOID)&status_nextcampaignmission_hook, (LPVOID*)&old_statusnextcampaignmission);
    MH_CreateHook((LPVOID)Offset::STARMAP_INIT, (LPVOID)&starmap_init_hook, (LPVOID*)&old_starmapinit);
    //MH_CreateHook((LPVOID)Offset::STARMAP_STARMAP, (LPVOID)&starmap_starmap_hook, (LPVOID*)&old_starmapstarmap);
    MH_CreateHook((LPVOID)Offset::STATUS_GETCAMPAIGNMISSION, (LPVOID)&status_getcampaignmission_hook, (LPVOID*)&old_statusgetcampaignmission);
    MH_CreateHook((LPVOID)Offset::STATUS_GETFREELANCEMISSION, (LPVOID)&status_getfreelancemission_hook, (LPVOID*)&old_statusgetfreelancemission);
    //MH_CreateHook((LPVOID)Offset::STARMAP_DTOR, (LPVOID)&starmap_dtor_hook, (LPVOID*)&old_starmapdtor);
    //MH_CreateHook((LPVOID)Offset::STARMAP_ONTOUCHEND, (LPVOID)&starmap_ontouchend_hook, (LPVOID*)&old_starmaptouchend);
    //MH_CreateHook((LPVOID)Offset::MODSTATION_LEAVESTATION, (LPVOID)&modstation_leavestation_hook, (LPVOID*)&old_modstationleavestation);
    MH_CreateHook((LPVOID)Offset::MODSTATION_ONUPDATE, (LPVOID)&modstation_onupdate_hook, (LPVOID*)&old_modstationonupdate);
    MH_CreateHook((LPVOID)Offset::LEVEL_CREATECAMPAIGNMISSION, (LPVOID)&level_createcampaignmission_hook, (LPVOID*)&old_levelcreatecampaignmission);
    MH_CreateHook((LPVOID)Offset::MGAME_ONUPDATE, (LPVOID)&mgame_onupdate_hook, (LPVOID*)&old_mgameonupdate);
    MH_CreateHook((LPVOID)Offset::DIALOGUEWINDOW_DTOR, (LPVOID)&dialoguewindow_dtor_hook, (LPVOID*)&old_dialoguewindowdtor);
    MH_CreateHook((LPVOID)Offset::STARMAP_DEPART, (LPVOID)&starmap_depart_hook, (LPVOID*)&old_starmapdepart);
    //MH_CreateHook((LPVOID)Offset::PLAYEREGO_CALCCOLLISION, (LPVOID)&playerego_calccollision_hook, (LPVOID*)&old_playeregocalccollision);
    MH_CreateHook((LPVOID)Offset::FILEREAD_LOADWEAPONPOSITIONS, (LPVOID)&fileread_loadweaponpositions_hook, (LPVOID*)&old_filereadloadweaponpositions);
    MH_CreateHook((LPVOID)Offset::IMAGEFACTORY_DRAWITEM, (LPVOID)&imagefactory_drawitem_hook, (LPVOID*)&old_imagefactorydrawitem);
    MH_EnableHook(MH_ALL_HOOKS);
}

/*
// TODO: do cutscene custom entity anchor support
 else if (state == 1002) {
            uintptr_t entitiylist = *(uintptr_t*)((uintptr_t)levelptr + 0xcc);
            if (entitiylist != 0) {
                uintptr_t* entities = *(uintptr_t**)(entitiylist + 4);
                void* npc = (void*)entities[1]; 
                
                if (npc != nullptr) {
                    float* npcc = *(float**)((uintptr_t)npc + 8);
                    
                    if (npcc != nullptr) {
                        float x = npcc[7];
                        float y = npcc[8];
                        float z = npcc[9];
                        camfloats[2] = 0.0f;
                        camfloats[3] = 0.0f;
                        camfloats[4] = 0.0f;
                        camPtrs[1] = (uintptr_t)npcc; 
                        uintptr_t func = (DWORD)Offset::TARGETFOLLOWCAMERA_SETPOSITION;
                        __asm {
                            mov ecx, cameraptr
                            call func
                        }
                    }
                }
            }
*/