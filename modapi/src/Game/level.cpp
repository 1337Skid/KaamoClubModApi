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

void Level::init(lua_State* lua_state)
{
    globals_appmanager = reinterpret_cast<Globals_appManager**>(Offset::GLOBALS_APPMANAGER);
    globals_status = reinterpret_cast<Globals_status**>(Offset::GLOBALS_STATUS);
    lstate = lua_state;
}

void Level::createradiomessage(const std::string& name, const std::string& content, sol::table imageinfo)
{
    uintptr_t address = 0x479678;
    MGame* mgame = reinterpret_cast<MGame*>((*globals_appmanager)->m_pCurrentModule);

    if (!mgame) {
        std::cout << "[-] Cannot call level::createradiomessage because MGame isn't init!" << std::endl;
        return;
    }
    SingleLevel* level = mgame->m_pLevel;
    if (!level) {
        std::cout << "[-] Cannot call level::createradiomessage because Level isn't init!" << std::endl;
        return;
    }
    if (!imageinfo) {
        std::cout << "[-] imageinfo can't be null!" << std::endl;
        return;
    }
    int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
    std::wstring namews(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, namews.data(), len);
    len = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, nullptr, 0);
    std::wstring contentws(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, contentws.data(), len);
    CustomRadioMsg crm;
    crm.name = namews;
    crm.content = contentws;
    crm.race = imageinfo["race"] ? imageinfo["race"] : 0;
    crm.hair = imageinfo["hair"] ? imageinfo["hair"] : 0;
    crm.eyes = imageinfo["eyes"] ? imageinfo["eyes"] : 0;
    crm.mouth = imageinfo["mouth"] ? imageinfo["mouth"] : 0;
    crm.armor = imageinfo["armor"] ? imageinfo["armor"] : 0;
    created_radiomessages.push_back(crm);
    int a3 = 0; // voice line
    int a2 = 8; // image (will be edited with the hook sooo)
    __asm {
        push a3
        push level
        mov ecx, a2
        call address
    }
}

// TODO: DialogueWindow::DialogueWindow(v18, (AbyssEngine::String *)v19, (AbyssEngine::String *)v20,CHAR_NIVELIAN_SECURITY); ??
void Level::createdialoguewindow(sol::table dialogueinfo)
{
    uintptr_t address_constructor = 0x405E2D;
    uintptr_t address_pause = 0x497564;
    uintptr_t address_pagesarraylength = Offset::CAMPAIGN_SUCCESS_LENGTHS;
    int* pagearraylength = reinterpret_cast<int*>(address_pagesarraylength);
    DWORD old;
    MGame* mgame = reinterpret_cast<MGame*>((*globals_appmanager)->m_pCurrentModule);
    ModStation* mstation = nullptr;
    SingleLevel* level = mgame->m_pLevel;
    SingleMission* mission = (*globals_status)->m_pMission;
    int oldcurrentcampaignmission = (*globals_status)->m_nCurrentCampaignMission;
    int pages = 0;
    int isstationdialogue = 0;
    
    if (!dialogueinfo.valid()) {
        std::cout << "[-] Cannot call level:CreateDialogueWindow() because dialogue info is empty!" << std::endl;
        return;
    }

    if (!dialogueinfo[1]["name"] || !dialogueinfo[1]["content"] || !dialogueinfo[1]["image"] || !dialogueinfo[1]["isplayer"]) {
        std::cout << "[-] Cannot call level:CreateDialogueWindow() because dialogue info is empty!" << std::endl;
        return;
    }
    // in a station
    if (!level) {
        isstationdialogue = 1;
        mstation = reinterpret_cast<ModStation*>((*globals_appmanager)->m_pCurrentModule);
    }
    if (!isstationdialogue && mgame->m_pDialogueWindow != nullptr)
        AbyssEngine::memory_free(mgame->m_pDialogueWindow);
    if (!isstationdialogue)
        mgame->m_pDialogueWindow = reinterpret_cast<DialogueWindow*>(AbyssEngine::memory_allocate(sizeof(DialogueWindow)));
    else
        mstation->m_pDialogueWindow = reinterpret_cast<DialogueWindow*>(AbyssEngine::memory_allocate(sizeof(DialogueWindow))); 
    if (!mission) {
        std::cout << "cannot find mission?" << std::endl;
        return;
    }
    dialogueinfo.for_each([&](sol::object key, sol::object val) {
        sol::table entry = val.as<sol::table>();
        CustomDialogueMsg msg;
        sol::table image = entry["image"];
        msg.race = image[1];
        msg.hair = image[2];
        msg.eyes = image[3];
        msg.mouth = image[4];
        msg.armor = image[5];
        msg.isplayer = entry["isplayer"];
        std::string name = entry["name"];
        std::string content = entry["content"];
        int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
        std::wstring namews(len, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, namews.data(), len);
        len = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, nullptr, 0);
        std::wstring contentws(len, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, contentws.data(), len);
        msg.name = namews;
        msg.content = contentws;
        created_dialoguemessages.push_back(msg);
        pages++;
    });
    (*globals_status)->m_nCurrentCampaignMission = 1;
    if (VirtualProtect(&pagearraylength[1], sizeof(int), PAGE_EXECUTE_READWRITE, &old)) {
        pagearraylength[1] = pages * 2; // * 2 because the game divides by 2 why? idk and I don't want to know
        VirtualProtect(&pagearraylength[1], sizeof(int), old, &old);
    }
    int dialogue_mode = 1; // 0 = briefing and 1 is for success and 1+ = gameover
    int pause_value = 1;
    DialogueWindow* dialoguewindow = nullptr;
    if (!isstationdialogue)
        dialoguewindow = mgame->m_pDialogueWindow;
    else
        dialoguewindow = mstation->m_pDialogueWindow;
    if (!isstationdialogue)
        __asm {
            mov eax, mgame
            movzx ecx, pause_value
            mov edx, address_pause
            call edx
        }
    __asm {
        push dialogue_mode
        push level
        push mission
        push dialoguewindow
        call address_constructor
    }
    if (!isstationdialogue)
        *(bool*)((uintptr_t)mgame + 0x5e) = true;
    else
        *(bool*)((uintptr_t)mstation + 0x65) = true;
    (*globals_status)->m_nCurrentCampaignMission = oldcurrentcampaignmission;
}

void Level::createcutscene(sol::table camerapoints)
{
    if (!camerapoints.valid()) {
        std::cout << "[-] Cannot call level:CreateCutScene() because camerapoints is empty!" << std::endl;
        return;
    }
    if (!camerapoints[1]["pos"] || !camerapoints[1]["duration"]) {
        std::cout << "[-] Cannot call level:CreateCutScene() because camerapoints is empty!" << std::endl;
        return;
    }
    created_cutscenepts.clear();
    cutscene_timer = 0.0f;
    cutscene_pointid = 0;
    cutscene_anchored = false;
    camerapoints.for_each([&](sol::object key, sol::object val) {
        if (!val.is<sol::table>())
            return;
        sol::table entry = val.as<sol::table>();
        CustomCutscenePoint campoint;
        sol::table pos = entry["pos"];
        campoint.x = pos[1];
        campoint.y = pos[2];
        campoint.z = pos[3];
        campoint.duration = static_cast<float>(entry["duration"]) * 1000.0f;
        campoint.shakeamount = entry.get<sol::optional<float>>("shake").value_or(0.0f);
        campoint.shakefrequency = entry.get<sol::optional<int>>("shakefrequency").value_or(0);
        created_cutscenepts.push_back(campoint);
    });
    created_cutscene = 1;
}

int* Level::createroute(sol::table pospoints)
{
    uintptr_t address = Offset::ROUTE_ROUTE;
    std::vector<std::array<float, 3>> coordinates;

    if (!pospoints.valid()) {
        std::cout << "[-] Cannot call level:CreateRoute() because pospoints is empty!" << std::endl;
        return nullptr;
    }
    for (auto& point : pospoints) {
        sol::table values = point.second.as<sol::table>();
        coordinates.push_back({values[1].get<float>(), values[2].get<float>(), values[3].get<float>()});
    }
    if (coordinates.empty()) {
        std::cout << "[-] Cannot call level:CreateRoute() because coordinates are empty!" << std::endl;
        return nullptr;
    }
    int size = 1 + coordinates.size() * 3;
    float* coordinatesbuf = reinterpret_cast<float*>(AbyssEngine::memory_allocate(sizeof(float) * size));
    memset(coordinatesbuf, 0, sizeof(float) * size);
    coordinatesbuf[0] = 0.0f;
    for (int i = 0; i < static_cast<int>(coordinates.size()); i++) {
        coordinatesbuf[1 + i * 3 + 0] = coordinates[i][0];
        coordinatesbuf[1 + i * 3 + 1] = coordinates[i][1];
        coordinatesbuf[1 + i * 3 + 2] = coordinates[i][2];
    }
    void* routeptr = reinterpret_cast<void*>(AbyssEngine::memory_allocate(0x20));
    memset(routeptr, 0, 0x20);
    int count = ((static_cast<int>(coordinates.size()) - 1) * 3 + 1);
    int coordsptr = reinterpret_cast<int>(coordinatesbuf);
    int returnvalue = 0;
    __asm {
        push count
        push coordsptr
        mov ebx, routeptr
        call address
        mov returnvalue, eax
    }
    AbyssEngine::memory_free(coordinatesbuf);
    return reinterpret_cast<int*>(routeptr);
}

sol::table Level::getentities(void)
{
    sol::state_view lua(lstate);
    sol::table result = lua.create_table();
    MGame* mgame = reinterpret_cast<MGame*>((*globals_appmanager)->m_pCurrentModule);
    SingleLevel* level = mgame->m_pLevel;
    
    if (!level) {
        std::cout << "[-] Cannot call level:GetEntities() because you aren't in a level!" << std::endl;
        return result;
    }
    unsigned int* entities = *reinterpret_cast<unsigned int**>(reinterpret_cast<uintptr_t>(level) + 0xCC); // TODO: make Level struct...
    std::cout << "entities (getentities): " << entities << std::endl;
    if (!entities)
        return result;
    int entitysize = entities[0];
    unsigned int* entitiesptr = reinterpret_cast<unsigned int*>(entities[1]);
    for (int i = 0; i < entitysize; i++) {
        if (entitiesptr[i])
            result.add(KIPlayer(reinterpret_cast<int*>(entitiesptr[i])));
    }
    return result;
}

// TODO: fix crash when mining the asteroid
void Level::createasteroid(float x, float y, float z, float scale, int meshid)
{
    uintptr_t address_aegeometry = Offset::AEGEOMETRY_AEGEOMETRY;
    uintptr_t address_setlodmeshes = Offset::AEGEOMETRY_SETLODMESHES;
    uintptr_t address_addobject = Offset::LODMANAGER_ADDOBJECT;
    uintptr_t address_playerasteroid = Offset::PLAYERASTEROID_PLAYERASTEROID;
    uintptr_t address_forceupdate = Offset::LODMANAGER_FORCEUPDATE;
    MGame* mgame = reinterpret_cast<MGame*>((*globals_appmanager)->m_pCurrentModule);

    if (!mgame) {
        std::cout << "[-] Cannot call level:CreateAsteroid() because you aren't in a level!" << std::endl;
        return;
    }
    SingleLevel* level = mgame->m_pLevel;
    void* geometry = reinterpret_cast<void*>(AbyssEngine::memory_allocate(192));
    memset(geometry, 0, 192);
    int a3 = 0;
    int geometryresult = 0;
    int geometryptr = reinterpret_cast<int>(geometry);
    __asm {
        push a3
        push meshid
        mov edi, geometryptr
        call address_aegeometry
        mov geometryresult, eax
    }
    if (!geometryresult) {
        std::cout << "[-] Failed to create geometry wtf?" << std::endl;
        return;
    }
    short lodmeshes[3] = { (short)(meshid), (short)(meshid), (short)(meshid) }; // LOD meshes so which mesh to show if the geometry is far of the camera (used for optimizations in games)
    int loddistances[3] = { 60000, 100000, 120000 };
    int lodmeshptr = reinterpret_cast<int>(lodmeshes);
    int loddinstanceptr = reinterpret_cast<int>(loddistances);
    int lodcount = 3;
    __asm {
        push lodcount
        push loddinstanceptr
        push lodmeshptr
        mov edi, geometryresult
        call address_setlodmeshes
    }
    int lodroot = *reinterpret_cast<int*>(level);
    __asm {
        push lodroot
        mov eax, geometryresult
        call address_addobject
        add esp, 4
    }
    int scalebits;
    memcpy(&scalebits, &scale, sizeof(float));
    float sizetierfloat = 4.0f;
    int sizetierbits;
    memcpy(&sizetierbits, &sizetierfloat, sizeof(float));
    void* obj = reinterpret_cast<void*>(AbyssEngine::memory_allocate(312));
    memset(obj, 0, 312);
    float pos[3] = { x, y, z };
    int posptr = reinterpret_cast<int>(pos);
    int objptr = reinterpret_cast<int>(obj);
    int levelptr = reinterpret_cast<int>(level);
    int faction = 0;
    int asteroidtype = 154;
    int unk = 0;
    int asteroidresult = 0;
    __asm {
        push unk
        push sizetierbits
        push scalebits
        push posptr
        push asteroidtype
        push faction
        push geometryresult
        push objptr
        mov ecx, geometryresult
        call address_playerasteroid
        mov asteroidresult, eax
    }
    // vtable+0x10 = set parent level
    __asm {
        mov ecx, asteroidresult
        mov eax, [ecx]
        push levelptr
        call dword ptr [eax + 0x10]
    }
    // createAsteroids sets 0x60C7F0 0x60C7F4 and 0x60C7F8 for the lod
    float* lx = reinterpret_cast<float*>(0x60C7F0);
    float* ly = reinterpret_cast<float*>(0x60C7F4);
    float* lz = reinterpret_cast<float*>(0x60C7F8);
    *lx = x;
    *ly = y;
    *lz = z;
    if (level->m_pAsteroids) {
        int count = level->m_pAsteroids->size;
        level->m_pAsteroids->data[count] = reinterpret_cast<int*>(asteroidresult);
        level->m_pAsteroids->size = count + 1;
    }
}

// TODO: hook playerfighter constructor for custom speed,hp etc...
void Level::createfighter(int meshid, int faction)
{
    Level::created_playerfighters.push_back({meshid, faction});
}

// just a quick reminder for me to look at rendering stuff for UIs etc..
//uintptr_t address_draw = 0x40764A;
//__asm { oh my fucking god do not call DialogueWindow::draw() you'll see it for one frame...
//    push dialoguewindow
//    call address_draw
//} instead set mgame+0x5e to true (MGame::OnRender2D) it took me a day to understand that LMAO