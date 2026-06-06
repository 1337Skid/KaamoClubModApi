#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <thread>
#include <chrono>
#include "offset.h"

void Asset::init()
{
    // Canvas is the last thing to load and I need to sleep if canvas is still nullptr
    uintptr_t base = MemoryUtils::GetModuleBase("GoF2.exe");

    while (globals_canvas == 0) {
        globals_canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);
        if (globals_canvas == nullptr) 
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    }
}

std::string Asset::getassetfilepath(unsigned int id)
{
    return MemoryUtils::ReadString(globals_canvas->m_pResources.data[id]->m_pResourceInfo->m_sResourcePath);
}

void Asset::setassetfilepath(unsigned int id, const std::string value)
{
    MemoryUtils::WriteString(globals_canvas->m_pResources.data[id]->m_pResourceInfo->m_sResourcePath, value);
}

std::string Asset::gettext(int id)
{
    AEString* returned_str = nullptr;
    uintptr_t address = Offset::GAMETEXT_GETTEXT;

    __asm {
        mov eax, id
        call address
        mov returned_str, eax
    }
    return ModApiUtils::w2s(returned_str->text);
}

int Asset::createtexture(const std::string& path)
{
    Globals_Canvas* canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);
    if (canvas == nullptr) {
        EarlyAssets asset;
        asset.id = next_texture_id++;
        asset.path = path;
        asset.textureid = -1;
        asset.regionid = -1;
        created_earlyassets.push_back(asset);
        return asset.id;
    }
    int id = next_texture_id++;
    auto& resources = canvas->m_pResources;
    auto *info = reinterpret_cast<ResourceInfo*>(AbyssEngine::memory_allocate(sizeof(ResourceInfo)));
    auto *buf = reinterpret_cast<char*>(AbyssEngine::memory_allocate(path.size() + 1));
    std::memcpy(buf, path.c_str(), path.size() + 1);
    info->m_sResourcePath = reinterpret_cast<uintptr_t>(buf);
    info->m_sTextureId = -1;
    auto *res = reinterpret_cast<Resource*>(AbyssEngine::memory_allocate(sizeof(Resource)));
    std::memset(res, 0, sizeof(Resource));
    res->m_nResourceId = static_cast<unsigned short>(id);
    res->field_4 = 2;
    res->field_8 = -1;
    res->m_pResourceInfo = info;
    int old = resources.size;
    auto **newdata = reinterpret_cast<Resource**>(AbyssEngine::memory_allocate(sizeof(Resource*) * (old + 1)));
    if (old > 0 && resources.data != nullptr)
        std::memcpy(newdata, resources.data, sizeof(Resource*) * old);
    newdata[old] = res;
    resources.data = newdata;
    resources.size = old + 1;
    resources.size2 = old + 1;
    return id;
}

int Asset::createsprite(int textureid, int regionid)
{
    Globals_Canvas* canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);
    if (canvas == nullptr) {
        EarlyAssets asset;
        asset.id = next_sprite_id++;
        asset.path = "";
        asset.textureid = textureid;
        asset.regionid = regionid;
        created_earlyassets.push_back(asset);
        return asset.id;
    }
    int id = next_sprite_id++;
    auto& resources = canvas->m_pResources;
    auto* ref = reinterpret_cast<short*>(AbyssEngine::memory_allocate(4));
    ref[0] = static_cast<short>(textureid);
    ref[1] = static_cast<short>(regionid);
    auto* res = reinterpret_cast<Resource*>(AbyssEngine::memory_allocate(sizeof(Resource)));
    std::memset(res, 0, sizeof(Resource));
    res->m_nResourceId = static_cast<unsigned short>(id);
    res->field_4 = 0;
    res->field_8 = -1;
    res->m_pResourceInfo = reinterpret_cast<ResourceInfo*>(ref);
    int old = resources.size;
    auto** newdata = reinterpret_cast<Resource**>(AbyssEngine::memory_allocate(sizeof(Resource*) * (old + 1)));
    if (old > 0 && resources.data != nullptr)
        std::memcpy(newdata, resources.data, sizeof(Resource*) * old);
    newdata[old] = res;
    resources.data = newdata;
    resources.size = old + 1;
    resources.size2 = old + 1;
    return id;
}

int Asset::createmesh(const std::string& path, int material_id)
{
    Globals_Canvas* canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);
    auto& resources = canvas->m_pResources; 
    int id = next_mesh_id++;
    auto *buf = reinterpret_cast<char*>(AbyssEngine::memory_allocate(path.size() + 1));
    
    std::memcpy(buf, path.c_str(), path.size() + 1);
    auto *mesh = reinterpret_cast<uint8_t*>(AbyssEngine::memory_allocate(0x8));
    *reinterpret_cast<uint32_t*>(mesh + 0x00) = reinterpret_cast<uint32_t>(buf); // TODO: do mesh struct
    *reinterpret_cast<unsigned short*>(mesh + 0x04) = static_cast<unsigned short>(material_id);
    *reinterpret_cast<uint8_t*>(mesh + 0x06) = 0;
    auto *res = reinterpret_cast<Resource*>(AbyssEngine::memory_allocate(sizeof(Resource)));
    std::memset(res, 0, sizeof(Resource));
    res->m_nResourceId = static_cast<unsigned short>(id);
    res->field_2 = 0;
    res->field_4 = 4;
    res->field_8 = -1;
    res->m_pResourceInfo = reinterpret_cast<ResourceInfo*>(mesh);
    int old = resources.size;
    auto **newdata = reinterpret_cast<Resource**>(AbyssEngine::memory_allocate(sizeof(Resource*) * (old + 1)));
    if (old > 0 && resources.data != nullptr)
        std::memcpy(newdata, resources.data, sizeof(Resource*) * old);
    newdata[old] = res;
    resources.data = newdata;
    resources.size = old + 1;
    resources.size2 = old + 1;
    return id;
}

int Asset::creatematerial(int diffuse, int normal, int shader)
{
    Globals_Canvas* canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);
    auto& resources = canvas->m_pResources; 
    int id = next_material_id++;
    auto *mat = reinterpret_cast<uint8_t*>(AbyssEngine::memory_allocate(40));
    
    // TODO: do material struct
    for (int i = 0; i < 8; i++)
        *reinterpret_cast<unsigned short*>(mat + i * 2) = -1;
    *reinterpret_cast<unsigned short*>(mat + 0x00) = static_cast<unsigned short>(diffuse);
    *reinterpret_cast<unsigned short*>(mat + 0x02) = static_cast<unsigned short>(normal);
    *reinterpret_cast<uint32_t*>(mat + 0x10) = static_cast<uint32_t>(shader);
    *reinterpret_cast<uint32_t*>(mat + 0x14) = 0;
    *reinterpret_cast<uint32_t*>(mat + 0x18) = 0;
    *reinterpret_cast<float*>(mat + 0x1C) = -10.0f;
    *reinterpret_cast<float*>(mat + 0x20) = 0.0f;
    *reinterpret_cast<float*>(mat + 0x24) = 0.0f;
    auto *res = reinterpret_cast<Resource*>(AbyssEngine::memory_allocate(sizeof(Resource)));
    std::memset(res, 0, sizeof(Resource));
    res->m_nResourceId = static_cast<unsigned short>(id);
    res->field_2 = 0;
    res->field_4 = 6;
    res->field_8 = -1;
    res->m_pResourceInfo = reinterpret_cast<ResourceInfo*>(mat);
    int old = resources.size;
    auto **newdata = reinterpret_cast<Resource**>(AbyssEngine::memory_allocate(sizeof(Resource*) * (old + 1)));
    if (old > 0 && resources.data != nullptr)
        std::memcpy(newdata, resources.data, sizeof(Resource*) * old);
    newdata[old] = res;
    resources.data = newdata;
    resources.size = old + 1;
    resources.size2 = old + 1;
    return id;
}

void Asset::drawstring(const std::string& text, int x, int y)
{
    int canvas = *reinterpret_cast<int*>(Offset::GLOBALS_CANVAS);
    int font = *reinterpret_cast<int*>(Offset::GLOBALS_FONT);
    std::wstring wtext = ModApiUtils::s2w(text);
    uintptr_t addr_drawstring = Offset::ABYSSENGINE_PAINTCANVAS_DRAWSTRING;

    AEString mystring;
    mystring.text = const_cast<wchar_t*>(wtext.c_str()); 
    mystring.size = wtext.length();
    AEString* pstring = &mystring;
    __asm {
        push y
        push x
        push pstring
        push font
        push canvas
        call addr_drawstring
    }
}

void Asset::setcolor(int r, int g, int b, int a)
{
    int canvas = *reinterpret_cast<int*>(Offset::GLOBALS_CANVAS);
    unsigned int color = ((a & 0xFF) << 24) | ((b & 0xFF) << 16) | ((g & 0xFF) << 8) | (r & 0xFF);
    uintptr_t addr_setcolor = Offset::ABYSSENGINE_PAINTCANVAS_SETCOLOR;
    
    __asm {
        mov eax, color
        mov ecx, canvas
        call addr_setcolor
    }
}

void Asset::drawimage2d(unsigned int id, int x, int y)
{
    int canvas = *reinterpret_cast<int*>(Offset::GLOBALS_CANVAS);
    uintptr_t addr_drawimage2d = Offset::ABYSSENGINE_PAINTCANVAS_DRAWIMAGE2D;
    uint8_t flags = 0;
    
    __asm {
        movzx eax, flags
        push eax
        push y
        push x
        push id
        push canvas
        mov eax, addr_drawimage2d
        call eax
    }
}

int Asset::image2dcreate(uint16_t id)
{
    unsigned int slot = -1;
    unsigned int* pslot = &slot;
    uintptr_t addr_image2dcreate = Offset::ABYSSENGINE_PAINTCANVAS_IMAGE2DCREATE;

    __asm {
        xor eax, eax
        mov ax, id
        push pslot
        mov edx, addr_image2dcreate
        call edx
    }
    return slot;
}