#ifndef ITEM_H
#define ITEM_H
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <MinHook.h>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "luamanager.h"
#include "abyssengine.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include "offset.h"

class Item {
    private:
        static inline Globals_status** globals_status = 0;
        struct ModdedItem {
            int id;
            std::string type;
            std::wstring name;
            std::wstring description;
            SingleItem item;
        };
    public:
        static inline std::vector<ModdedItem> created_items;
        static void init(void);
        static int create(const std::string& name, const std::string& description, sol::table iteminfo);
        static void edit(void);
        static void unlockblueprint(int id);
        static void lockblueprint(int id);

        static void refreshitemsprices(void);
        static void refreshblueprints(void);
        static void create_gun_item(ModdedItem m, int damage, int loadingspeed, int range, int speed);
        static void create_booster_item(ModdedItem m, int boostduration, int loadingspeed, int effect);
        static void create_cloak_item(ModdedItem m, int effect, int loadingspeed, int energyconsumption);
        static void create_commodity_item(ModdedItem m);
        static void create_blueprint(SingleItem& item, const std::vector<int>& ingredients, const std::vector<int>& ingredients_count);
};
#endif