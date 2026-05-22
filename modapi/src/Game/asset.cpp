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
    uintptr_t address = 0x004F38B0;

    __asm {
        mov eax, id
        call address
        mov returned_str, eax
    }
    return ModApiUtils::w2s(returned_str->text);
}

int Asset::createtexture(const std::string& path)
{
    Globals_Canvas* canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);;
    auto& resources = canvas->m_pResources; 
    int id = next_texture_id++;
    auto* info = reinterpret_cast<ResourceInfo*>(AbyssEngine::memory_allocate(sizeof(ResourceInfo)));    
    auto* buf = reinterpret_cast<char*>(AbyssEngine::memory_allocate(path.size() + 1));

    std::memcpy(buf, path.c_str(), path.size() + 1);
    info->m_sResourcePath = reinterpret_cast<uintptr_t>(buf);
    info->m_sTextureId = -1;
    auto* res = reinterpret_cast<Resource*>(AbyssEngine::memory_allocate(sizeof(Resource)));
    std::memset(res, 0, sizeof(Resource));
    res->m_nResourceId = static_cast<unsigned short>(id);
    res->field_2 = 0;
    res->field_4 = 2;
    res->field_8 = -1;
    res->m_pResourceInfo = info;
    int old = resources.size;
    auto** newdata = reinterpret_cast<Resource**>(AbyssEngine::memory_allocate(sizeof(Resource*) * (old + 1)));
    if (old > 0 && resources.data != nullptr)
        std::memcpy(newdata, resources.data, sizeof(Resource*) * old);
    newdata[old] = res;
    resources.data  = newdata;
    resources.size  = (old + 1);
    resources.size2 = (old + 1);
    return id;
}

int Asset::createmesh(const std::string& path, int material_id)
{
    Globals_Canvas* canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);
    auto& resources = canvas->m_pResources; 
    int id = next_mesh_id++;
    auto* buf = reinterpret_cast<char*>(AbyssEngine::memory_allocate(path.size() + 1));
    
    std::memcpy(buf, path.c_str(), path.size() + 1);
    auto* mesh = reinterpret_cast<uint8_t*>(AbyssEngine::memory_allocate(0x8));
    *reinterpret_cast<uint32_t*>(mesh + 0x00) = reinterpret_cast<uint32_t>(buf); // TODO: do mesh struct
    *reinterpret_cast<unsigned short*>(mesh + 0x04) = static_cast<unsigned short>(material_id);
    *reinterpret_cast<uint8_t*>(mesh + 0x06) = 0;
    auto* res = reinterpret_cast<Resource*>(AbyssEngine::memory_allocate(sizeof(Resource)));
    std::memset(res, 0, sizeof(Resource));
    res->m_nResourceId = static_cast<unsigned short>(id);
    res->field_2 = 0;
    res->field_4 = 4;
    res->field_8 = -1;
    res->m_pResourceInfo = reinterpret_cast<ResourceInfo*>(mesh);
    int old = resources.size;
    auto** newdata = reinterpret_cast<Resource**>(AbyssEngine::memory_allocate(sizeof(Resource*) * (old + 1)));
    if (old > 0 && resources.data != nullptr)
        std::memcpy(newdata, resources.data, sizeof(Resource*) * old);
    newdata[old] = res;
    resources.data  = newdata;
    resources.size  = (old + 1);
    resources.size2 = (old + 1);
    return id;
}

int Asset::creatematerial(int diffuse, int normal, int shader)
{
    Globals_Canvas* canvas = *reinterpret_cast<Globals_Canvas**>(Offset::GLOBALS_CANVAS);
    auto& resources = canvas->m_pResources; 
    int id = next_material_id++;
    auto* mat = reinterpret_cast<uint8_t*>(AbyssEngine::memory_allocate(40));
    
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
    auto* res = reinterpret_cast<Resource*>(AbyssEngine::memory_allocate(sizeof(Resource)));
    std::memset(res, 0, sizeof(Resource));
    res->m_nResourceId = static_cast<unsigned short>(id);
    res->field_2 = 0;
    res->field_4 = 6;
    res->field_8 = -1;
    res->m_pResourceInfo = reinterpret_cast<ResourceInfo*>(mat);
    int old = resources.size;
    auto** newdata = reinterpret_cast<Resource**>(AbyssEngine::memory_allocate(sizeof(Resource*) * (old + 1)));
    if (old > 0 && resources.data != nullptr)
        std::memcpy(newdata, resources.data, sizeof(Resource*) * old);
    newdata[old] = res;
    resources.data  = newdata;
    resources.size  = (old + 1);
    resources.size2 = (old + 1);
    return id;
}