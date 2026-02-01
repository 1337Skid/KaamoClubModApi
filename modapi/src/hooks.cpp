#include "patches.h"
#include "abyssengine.h"
#include "hooks.h"
#include <Game/system.h>
#include <Game/level.h>
#include <iostream>
#include <vector>

Hooks::globals_init Hooks::oldglobals_init = nullptr;
Hooks::fileread_loadstationbinaryfromid Hooks::old_filereadloadstationbinaryfromid = nullptr;
Hooks::fileread_loadstationbinary Hooks::old_filereadloadstationbinary = nullptr;
Hooks::standing_isenemy Hooks::old_standingisenemy = nullptr;
Hooks::abyssengine_paintcanvas_setcolor Hooks::old_abyssenginepaintcanvassetcolor = nullptr;
Hooks::gametext_gettext Hooks::old_gametextgettext = nullptr;
Hooks::recordhandler_recordstorewrite Hooks::old_recordhandlerrecordstorewrite = nullptr;
Hooks::level_creategun Hooks::old_levelcreategun = nullptr;
Hooks::level_createship Hooks::old_levelcreateship = nullptr;
Hooks::imagefactory_drawchar Hooks::old_imagefactorydrawchar = nullptr;

void Hooks::injectitems()
{
    Items* itemsarray = (Items*)GLOBALS_ITEMS;
    AEArray<SingleItem*>* old_array = itemsarray->items;
    int newcount = old_array->size + (int)Item::created_items.size();
    AEArray<SingleItem*>* new_array = (AEArray<SingleItem*>*)AbyssEngine::memory_allocate(sizeof(AEArray<SingleItem*>));
    new_array->size = newcount;
    new_array->size2 = newcount;
    new_array->data = (SingleItem**)AbyssEngine::memory_allocate(sizeof(SingleItem*) * newcount);
    memcpy(new_array->data, old_array->data, sizeof(SingleItem*) * old_array->size);
    for (int i = 0; i < (int)Item::created_items.size(); i++) {
        SingleItem* item = (SingleItem*)AbyssEngine::memory_allocate(sizeof(SingleItem));        
        *item = Item::created_items[i].item;        
        new_array->data[old_array->size + i] = item;
    }
    itemsarray->items = new_array;
    // I'm not freeing old_array->data[i]->m_pItemInfo array because I did a memcpy of old array to new array and I don't want to realloc every item info it's kinda useless
    AbyssEngine::memory_free(old_array->data);
    AbyssEngine::memory_free(old_array);    
}

void Hooks::injectsystemsandstations()
{
    auto* galaxy = *(Galaxy**)GLOBALS_GALAXY;
    if (!galaxy || !galaxy->systems || System::created_systems.empty())
        return;
    auto* systems = galaxy->systems;
    const uint32_t sys_count = systems->size;
    const uint32_t add_sys_count = (uint32_t)System::created_systems.size();
    const uint32_t new_sys_count = sys_count + add_sys_count;
    auto* new_sys_array = reinterpret_cast<AEArray<SingleSystem*>*>(AbyssEngine::memory_allocate(sizeof(AEArray<SingleSystem*>)));
    new_sys_array->size = new_sys_array->size2 = new_sys_count;
    new_sys_array->data = reinterpret_cast<SingleSystem**>(AbyssEngine::memory_allocate(sizeof(SingleSystem*) * new_sys_count));

    memcpy(new_sys_array->data, systems->data, sizeof(SingleSystem*) * sys_count);
    for (size_t i = 0; i < System::created_systems.size(); ++i) {
        const uint32_t sysid = sys_count + (uint32_t)i;
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
            newsys->station_ids = AbyssEngine::newarray<uint32_t>((int)matching_ids.size());
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
    Patches::patchstarmap((uint8_t)new_sys_count); 
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
            int count = (int)matchlist.size();
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
    uint32_t requested_id = (uint32_t)*id;

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
            Hooks::injectsystemsandstations();
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

    __asm {
        mov id, eax
        mov edx, [ebp + 4]
        mov returnaddr, edx
    }
    // Custom radio messages texts
    if ((uintptr_t)returnaddr == 0x4bcc59 && !Level::created_radiomessages.empty()) {
        static AEString customstring;
        if (Level::created_radiomessages.size() == 1) {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[0].name.c_str());
            customstring.size = (uint32_t)Level::created_radiomessages[0].name.length();
        } else {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[Level::created_radiomessages.size() - 1].name.c_str());
            customstring.size = (uint32_t)Level::created_radiomessages[Level::created_radiomessages.size() - 1].name.length();
        }
        return &customstring;
    }
    if ((uintptr_t)returnaddr == 0x4bca93 && !Level::created_radiomessages.empty()) {
        static AEString customstring;
        if (Level::created_radiomessages.size() == 1) {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[0].content.c_str());
            customstring.size = (uint32_t)Level::created_radiomessages[0].content.length();
        } else {
            customstring.text = const_cast<wchar_t*>(Level::created_radiomessages[Level::created_radiomessages.size() - 1].content.c_str());
            customstring.size = (uint32_t)Level::created_radiomessages[Level::created_radiomessages.size() - 1].content.length();
        }
        return &customstring;
    }
    // The return addresses of every GameText::GetText() items name, useful to not overwrite in others strings
    // TODO: find in game npc cargo hold returnaddr
    // Custom items texts
    if ((uintptr_t)returnaddr == 0x483331 || (uintptr_t)returnaddr == 0x454c4e || (uintptr_t)returnaddr == 0x44edae || (uintptr_t)returnaddr == 0x4c9725 || (uintptr_t)returnaddr == 0x4cc0bc || (uintptr_t)returnaddr == 0x48d8b1 || (uintptr_t)returnaddr == 0x458618 || (uintptr_t)returnaddr == 0x4585C1) {
        for (const auto& item : Item::created_items) {
            if (id == item.id + 1247) { // + 1247 bcz the game decided so don't ask my why
                static AEString customstring;
                customstring.text = const_cast<wchar_t*>(item.name.c_str());
                customstring.size = (uint32_t)item.name.length();
                return &customstring;
            }
        }
    }
    // if ((uintptr_t)returnaddr == 0x45565e) { -- for ships names
    if ((uintptr_t)returnaddr == 0x48307b) {
        for (const auto& item : Item::created_items) {
            if (id == item.id + 1014) { // + 1014 bcz the game decided
                static AEString customstring;
                customstring.text = const_cast<wchar_t*>(item.description.c_str());
                customstring.size = (uint32_t)item.description.length();
                return &customstring;
            }
        }
    }
    __asm {
        mov eax, id
        call old_gametextgettext
    }
}

int __stdcall Hooks::recordhandler_recordstorewrite_hook(uintptr_t a, int b)
{
    // This function is useful to prevent custom items being injected in the save resulting in a crash at load...
    Globals_status** globals_status_ptr = (Globals_status**)0x60AD6C;
    Globals_status* globals_status = *globals_status_ptr;
    int oldsize = 196;

    if (Item::created_items.size() == 0)
        return old_recordhandlerrecordstorewrite(a, b);
    if (globals_status->m_pItemLowestPrices && globals_status->m_pItemLowestPrices->size > oldsize) {
        AEArray<unsigned int>* oldarray = globals_status->m_pItemLowestPrices;
        AEArray<unsigned int>* newarray = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));

        newarray->size = oldsize;
        newarray->size2 = oldsize;
        newarray->data = (unsigned int*)AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize);
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
        newarray->data = (unsigned int*)AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize);
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
        newarray->data = (unsigned int*)AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize);
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
        newarray->data = (unsigned int*)AbyssEngine::memory_allocate(sizeof(unsigned int) * oldsize);
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
    for (int i = 0; i < (int)Item::created_items.size(); i++) {
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
    //std::cout << a1 << std::endl;
    //std::cout << a2 << std::endl;
    //std::cout << a3 << std::endl;
    //std::cout << a4 << std::endl;
    //std::cout << returnaddr << std::endl;
    //std::cout << "---------------" << std::endl;
    AEArray<ImagePart*>* imageparts = reinterpret_cast<AEArray<ImagePart*>*>(a1);
    //for (int i = 0; i < imageparts->size; i++)
        //std::cout << imageparts->data[i]->id << std::endl;
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

void Hooks::init()
{
    MH_Initialize();
    MH_CreateHook((LPVOID)GLOBALS_INIT_ADDR, &globals_init_hook, (LPVOID*)&oldglobals_init);
    MH_CreateHook((LPVOID)FILEREAD_LOADSTATIONBINARYFROMID, &fileread_loadstationbinaryfromid_hook, (LPVOID*)&old_filereadloadstationbinaryfromid);
    MH_CreateHook((LPVOID)FILEREAD_LOADSTATIONBIRARY, &fileread_loadstationbinary_hook, (LPVOID*)&old_filereadloadstationbinary);
    //MH_CreateHook((LPVOID)STANDING_ISENEMY, &standing_isenemy_hook, (LPVOID*)&old_standingisenemy);
    MH_CreateHook((LPVOID)ABYSSENGINE_PAINTCANVAS_SETCOLOR, &abyssengine_paintcanvas_setcolor_hook, (LPVOID*)&old_abyssenginepaintcanvassetcolor);
    MH_CreateHook((LPVOID)GAMETEXT_GETTEXT, &gametext_gettext_hook, (LPVOID*)&old_gametextgettext);
    MH_CreateHook((LPVOID)RECORDHANDLER_RECORDSTOREWRITE, &recordhandler_recordstorewrite_hook, (LPVOID*)&old_recordhandlerrecordstorewrite);
    MH_CreateHook((LPVOID)LEVEL_CREATEGUN, &level_creategun_hook, (LPVOID*)&old_levelcreategun);
    //MH_CreateHook((LPVOID)LEVEL_CREATESHIP, &level_createship_hook, (LPVOID*)&old_levelcreateship);
    MH_CreateHook((LPVOID)IMAGEFACTORY_DRAWCHAR, &imagefactory_drawchar_hook, (LPVOID*)&old_imagefactorydrawchar);
    MH_EnableHook(MH_ALL_HOOKS);
}