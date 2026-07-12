#include <Game/ship.h>

int Ship::create(const std::string& name, const std::string& description, sol::table shipinfo, int tex_diffuse, int tex_normal, int material, int lod0, int lod1, int lod2)
{
    if (!shipinfo) {
        std::cout << "[-] shipinfo can't be null!" << std::endl;
        return -1;
    }
    int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
    std::wstring namews(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, namews.data(), len);
    len = MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, nullptr, 0);
    std::wstring descriptionws(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, descriptionws.data(), len);
    float handling = shipinfo["handling"] ? shipinfo["handling"] : 1.0f;
    CustomShip cs;
    cs.name = namews;
    cs.description = descriptionws;
    cs.id = created_ships.size() + 44;
    cs.lod0 = lod0;
    cs.lod1 = lod1;
    cs.lod2 = lod2;
    cs.maxhealth = shipinfo["maxhealth"] ? shipinfo["maxhealth"] : 1;
    cs.baseprice = shipinfo["baseprice"] ? shipinfo["baseprice"] : 1;
    cs.maxcargo = shipinfo["maxcargo"] ? shipinfo["maxcargo"] : 1;
    cs.armor = shipinfo["armor"] ? shipinfo["armor"] : 1;
    cs.handling = (handling + 1) / 100;
    cs.primaryslots = shipinfo["primaryslots"] ? shipinfo["primaryslots"] : 1;
    cs.secondaryslots = shipinfo["secondaryslots"] ? shipinfo["secondaryslots"] : 1;
    cs.turretslots = shipinfo["turretslots"] ? shipinfo["turretslots"] : 1;
    cs.equipmentslots = shipinfo["equipmentslots"] ? shipinfo["equipmentslots"] : 1;
    cs.hangar_y = shipinfo["hangar_y"] ? shipinfo["hangar_y"] : 1;
    cs.spriteicon = shipinfo["spriteicon"] ? shipinfo["spriteicon"] : 0;
    sol::table primary = shipinfo["primary_positions"];
    if (primary.valid()) {
        for (int i = 1; i <= primary.size(); i++) {
            sol::table p = primary[i];
            cs.primary_positions.push_back({ p[1], p[2], p[3] });
        }
    } else {
        cs.primary_positions = {{1.0f, 1.0f, 1.0f}};
    }
    sol::table secondary = shipinfo["secondary_positions"];
    if (secondary.valid()) {
        for (size_t i = 1; i <= secondary.size(); i++) {
            sol::table p = secondary[i];
            cs.secondary_positions.push_back({ p[1], p[2], p[3] });
        }
    } else {
        cs.secondary_positions = {{1.0f, 1.0f, 1.0f}};
    }
    sol::table turret = shipinfo["turret_positions"];
    if (turret.valid()) {
        for (size_t i = 1; i <= turret.size(); i++) {
            sol::table p = turret[i];
            cs.turret_positions.push_back({ p[1], p[2], p[3] });
        }
    } else {
        cs.turret_positions = {{1.0f, 1.0f, 1.0f}};
    }
    sol::table engines = shipinfo["engines"];
    if (engines.valid()) {
        for (int i = 1; i <= engines.size(); i++) {
            sol::table luarow = engines[i];
            sol::table p = luarow["pos"];
            sol::table intensity = luarow["intensity"];
            EnginesPos pos;
            pos.position  = { p[1], p[2], p[3] };
            pos.intensity = { intensity[1], intensity[2], intensity[3] };
            cs.engines.push_back(pos);
        }
    } else {
        EnginesPos pos;
        pos.position  = {1.0f, 1.0f, 1.0f};
        pos.intensity = {1.0f, 1.0f, 1.0f};
        cs.engines.push_back(pos);
    }
    created_ships.push_back(cs);
    return cs.id;
}