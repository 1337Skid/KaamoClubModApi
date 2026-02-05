#include <Game/item.h>

void Item::create_commodity_item(ModdedItem m)
{
    AEArray<ItemInfo>* iteminfoarr = (AEArray<ItemInfo>*)AbyssEngine::memory_allocate(sizeof(AEArray<ItemInfo>));
    iteminfoarr->size = 20;
    iteminfoarr->size2 = 20;
    iteminfoarr->data = (ItemInfo*)AbyssEngine::memory_allocate(sizeof(ItemInfo));
    memset(iteminfoarr->data, 0, sizeof(ItemInfo));
    ItemInfo* info = &iteminfoarr->data[0];
    info->field_0 = 0;
    info->m_nID = static_cast<int>(created_items.size() + 196);
    info->m_bIsThermo = 1;
    info->m_nType = m.item.m_nType;
    info->field_10 = 2;
    info->m_nSubType = m.item.m_nSubType;
    info->field_18 = 3;
    info->m_nTechLevel = m.item.m_nTechLevel;
    info->m_nHighestPriceSystemId = m.item.m_nHighestPriceSystemId;
    info->m_nLowestPriceSystemId = m.item.m_nLowestPriceSystemId;
    info->field_28 = 5;
    info->field_2C = 4;
    info->field_30 = 6;
    info->field_34 = 50;
    info->field_38 = 7;
    info->field_3C = 4;
    info->field_40 = 8;
    info->m_nPrice = m.item.m_nPrice;
    info->m_nPropertyOne = 44;
    info->m_nPropertyOneValue = 0;
    m.item.m_pItemInfo = iteminfoarr;
    created_items.push_back(m);
}

void Item::create_cloak_item(ModdedItem m, int effect, int loadingspeed, int energyconsumption)
{
    AEArray<ItemInfo>* iteminfoarr = (AEArray<ItemInfo>*)AbyssEngine::memory_allocate(sizeof(AEArray<ItemInfo>));
    iteminfoarr->size = 28;
    iteminfoarr->size2 = 28;
    iteminfoarr->data = (ItemInfo*)AbyssEngine::memory_allocate(sizeof(ItemInfo));
    memset(iteminfoarr->data, 0, sizeof(ItemInfo));
    ItemInfo* info = &iteminfoarr->data[0];
    info->field_0 = 0;
    info->m_nID = static_cast<int>(created_items.size() + 196);
    info->m_bIsThermo = 1;
    info->m_nType = m.item.m_nType;
    info->field_10 = 2;
    info->m_nSubType = m.item.m_nSubType;
    info->field_18 = 3;
    info->m_nTechLevel = m.item.m_nTechLevel;
    info->m_nHighestPriceSystemId = m.item.m_nHighestPriceSystemId;
    info->m_nLowestPriceSystemId = m.item.m_nLowestPriceSystemId;
    info->field_28 = 5;
    info->field_2C = 15;
    info->field_30 = 6;
    info->field_34 = 20;
    info->field_38 = 7;
    info->field_3C = 38000;
    info->field_40 = 8;
    info->m_nPrice = m.item.m_nPrice;
    info->m_nPropertyOne = 35;
    info->m_nPropertyOneValue = effect;
    info->m_nPropertyTwo = 36;
    info->m_nPropertyTwoValue = loadingspeed;
    info->m_nPropertyThree = 38;
    info->m_nPropertyThreeValue = energyconsumption;
    info->m_nPropertyFour = 44;
    info->m_nPropertyFourValue = 1;
    info->m_nPropertyFive = 45;
    info->m_nPropertyFiveValue = 14;
    m.item.m_pItemInfo = iteminfoarr;
    created_items.push_back(m);
}

void Item::create_booster_item(ModdedItem m, int boostduration, int loadingspeed, int effect)
{
    AEArray<ItemInfo>* iteminfoarr = (AEArray<ItemInfo>*)AbyssEngine::memory_allocate(sizeof(AEArray<ItemInfo>));
    iteminfoarr->size = 28;
    iteminfoarr->size2 = 28;
    iteminfoarr->data = (ItemInfo*)AbyssEngine::memory_allocate(sizeof(ItemInfo));
    memset(iteminfoarr->data, 0, sizeof(ItemInfo));
    ItemInfo* info = &iteminfoarr->data[0];
    info->field_0 = 0;
    info->m_nID = static_cast<int>(created_items.size() + 196);
    info->m_bIsThermo = 1;
    info->m_nType = m.item.m_nType;
    info->field_10 = 2;
    info->m_nSubType = m.item.m_nSubType;
    info->field_18 = 3;
    info->m_nTechLevel = m.item.m_nTechLevel;
    info->m_nHighestPriceSystemId = m.item.m_nHighestPriceSystemId;
    info->m_nLowestPriceSystemId = m.item.m_nLowestPriceSystemId;
    info->field_28 = 5;
    info->field_2C = 4;
    info->field_30 = 6;
    info->field_34 = 0;
    info->field_38 = 7;
    info->field_3C = 72500;
    info->field_40 = 8;
    info->m_nPrice = m.item.m_nPrice;
    info->m_nPropertyOne = 25;
    info->m_nPropertyOneValue = effect;
    info->m_nPropertyTwo = 26;
    info->m_nPropertyTwoValue = loadingspeed;
    info->m_nPropertyThree = 27;
    info->m_nPropertyThreeValue = boostduration;
    info->m_nPropertyFour = 44;
    info->m_nPropertyFourValue = 0;
    info->m_nPropertyFive = 45;
    info->m_nPropertyFiveValue = 107;
    m.item.m_pItemInfo = iteminfoarr;
    created_items.push_back(m);
}

void Item::create_gun_item(ModdedItem m, int damage, int loadingspeed, int range, int speed)
{
    AEArray<ItemInfo>* iteminfoarr = (AEArray<ItemInfo>*)AbyssEngine::memory_allocate(sizeof(AEArray<ItemInfo>));
    iteminfoarr->size = 30;
    iteminfoarr->size2 = 30;
    iteminfoarr->data = (ItemInfo*)AbyssEngine::memory_allocate(sizeof(ItemInfo));
    memset(iteminfoarr->data, 0, sizeof(ItemInfo));
    ItemInfo* info = &iteminfoarr->data[0];
    info->field_0 = 0;
    info->m_nID = static_cast<int>(created_items.size() + 196);
    info->m_bIsThermo = 1;
    info->m_nType = m.item.m_nType;
    info->field_10 = 2;
    info->m_nSubType = m.item.m_nSubType;
    info->field_18 = 3;
    info->m_nTechLevel = m.item.m_nTechLevel;
    info->m_nHighestPriceSystemId = m.item.m_nHighestPriceSystemId;
    info->m_nLowestPriceSystemId = m.item.m_nLowestPriceSystemId;
    info->field_28 = 5;
    info->field_2C = (info->m_nSubType == 1) ? 4 : 7;
    info->field_30 = 6;
    info->field_34 = (info->m_nSubType == 1) ? 10 : 90;
    info->field_38 = 7;
    info->field_3C = (info->m_nSubType == 1) ? 89000 : 2000;
    info->field_40 = 8;
    info->m_nPrice = m.item.m_nPrice;
    info->m_nPropertyOne = 9;
    info->m_nPropertyOneValue = damage;
    info->m_nPropertyTwo = 11;
    info->m_nPropertyTwoValue = loadingspeed;
    info->m_nPropertyThree = 12;
    info->m_nPropertyThreeValue = range;
    info->m_nPropertyFour = 13;
    info->m_nPropertyFourValue = speed;
    info->m_nPropertyFive = 44;
    info->m_nPropertyFiveValue = 0;
    info->field_70 = 45;
    info->field_74 = (info->m_nSubType == 1) ? 53 : 52;
    m.item.m_pItemInfo = iteminfoarr;
    created_items.push_back(m);
}

int Item::create(const std::string& name, const std::string& description, sol::table iteminfo) {
    if (EventManager::isearlyinit_finished) {
        std::cout << "[-] Failed to call item:Create()" << std::endl;
        return -1;
    }
    if (!iteminfo || !iteminfo["type"]) {
        std::cout << "[-] Cannot create custom item because iteminfo or iteminfo type is null" << std::endl;
        return -1;
    }
    int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
    std::wstring namews(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, namews.data(), len);
    len = MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, nullptr, 0);
    std::wstring descriptionws(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, descriptionws.data(), len);

    ModdedItem m;
    m.id = static_cast<int>(created_items.size() + 196);
    m.name = namews;
    m.description = descriptionws;
    memset(&m.item, 0, sizeof(SingleItem));
    m.item.m_nID = m.id;
    m.type = iteminfo["type"].get<std::string>();
    if (m.type == "Laser" || m.type == "LaserBeam" || m.type == "Blaster")
        m.item.m_nType = 0;
    else if (m.type == "Booster" || m.type == "Cloak")
        m.item.m_nType = 3;
    else if (m.type == "Commodity")
        m.item.m_nType = 4;
    if (m.type == "Laser" || m.type == "LaserBeam")
        m.item.m_nSubType = 0;
    if (m.type == "Blaster")
        m.item.m_nSubType = 1;
    else if (m.type == "Booster")
        m.item.m_nSubType = 14;
    else if (m.type == "Cloak")
        m.item.m_nSubType = 21;
    else if (m.type == "Commodity")
        m.item.m_nSubType = 22;
    m.item.m_nTechLevel = iteminfo["techlevel"] ? iteminfo["techlevel"] : 1;
    m.item.m_nLowestPriceSystemId = 14; //iteminfo["lowestprice_systemid"] ? iteminfo["lowestprice_systemid"] : 1; TODO: commented bcz its doing weird behaviour
    m.item.m_nHighestPriceSystemId = 4; //iteminfo["highestprice_systemid"] ? iteminfo["highestprice_systemid"] : 1;
    m.item.m_nPrice = iteminfo["baseprice"] ? iteminfo["baseprice"] : 1;
    m.item.m_nMinPrice = iteminfo["minprice"] ? iteminfo["minprice"] : 1;
    m.item.m_nMaxPrice = iteminfo["maxprice"] ? iteminfo["maxprice"] : 1;
    m.item.field_34 = 0;
    m.item.field_38 = 0;
    m.item.field_3C = 0;
    m.item.field_40 = 0;
    //m.item.m_bDisableSelling = 0; // not sure about this one
    if (m.item.m_nType == 0)
        create_gun_item(m, 
            iteminfo["damage"] ? iteminfo["damage"] : 1, 
            iteminfo["loadingspeed"] ? iteminfo["loadingspeed"] : 1,
            iteminfo["range"] ? iteminfo["range"] : 1,
            iteminfo["speed"] ? iteminfo["speed"] : 1
        );
    else if (m.item.m_nType == 3 && m.item.m_nSubType == 14)
        create_booster_item(m,
            iteminfo["boostduration"] ? iteminfo["boostduration"] : 1,
            iteminfo["loadingspeed"] ? iteminfo["loadingspeed"] : 1,
            iteminfo["effect"] ? iteminfo["effect"] : 1
        );
    else if (m.item.m_nType == 3 && m.item.m_nSubType == 21)
        create_cloak_item(m,
            iteminfo["effect"] ? iteminfo["effect"] : 1,
            iteminfo["loadingspeed"] ? iteminfo["loadingspeed"] : 1,
            iteminfo["energyconsumption"] ? iteminfo["energyconsumption"] : 1
        );
    else if (m.item.m_nType == 4)
        create_commodity_item(m);
    return m.id;
}

void Item::refreshitemsprices() {
    Globals_status** globals_status_ptr = reinterpret_cast<Globals_status**>(0x0060AD6C);
    Globals_status* globals_status = *globals_status_ptr;
    int old_size = 196;
    int newsize = old_size + static_cast<int>(created_items.size());

    if(created_items.size() == 0)
        return;

    AEArray<unsigned int>* oldlow = globals_status->m_pItemLowestPrices;
    AEArray<unsigned int>* newlow = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));
    newlow->size = newsize;
    newlow->size2 = newsize;
    newlow->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * newsize));
    memcpy(newlow->data, oldlow->data, sizeof(unsigned int) * old_size);
    for (int i = 0; i < static_cast<int>(created_items.size()); i++)
        newlow->data[old_size + i] = created_items[i].item.m_nMinPrice;
    globals_status->m_pItemLowestPrices = newlow;
    AbyssEngine::memory_free(oldlow->data);
    AbyssEngine::memory_free(oldlow);

    AEArray<unsigned int>* oldhigh = globals_status->m_pItemHighestPrices;
    AEArray<unsigned int>* newhigh = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));
    newhigh->size = newsize;
    newhigh->size2 = newsize;
    newhigh->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * newsize));
    memcpy(newhigh->data, oldhigh->data, sizeof(unsigned int) * old_size);
    for (int i = 0; i < static_cast<int>(created_items.size()); i++)
        newhigh->data[old_size + i] = created_items[i].item.m_nMaxPrice;
    globals_status->m_pItemHighestPrices = newhigh;
    AbyssEngine::memory_free(oldhigh->data);
    AbyssEngine::memory_free(oldhigh);

    AEArray<unsigned int>* oldlowsys = globals_status->m_pItemLowestPricesSystem;
    AEArray<unsigned int>* newlowsys = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));
    newlowsys->size = newsize;
    newlowsys->size2 = newsize;
    newlowsys->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * newsize));
    memcpy(newlowsys->data, oldlowsys->data, sizeof(unsigned int) * old_size);
    for (int i = 0; i < static_cast<int>(created_items.size()); i++)
        newlowsys->data[old_size + i] = created_items[i].item.m_nLowestPriceSystemId;
    globals_status->m_pItemLowestPricesSystem = newlowsys;
    AbyssEngine::memory_free(oldlowsys->data);
    AbyssEngine::memory_free(oldlowsys);

    AEArray<unsigned int>* oldhighsys = globals_status->m_pItemHighestPricesSystem;
    AEArray<unsigned int>* newhighsys = (AEArray<unsigned int>*)AbyssEngine::memory_allocate(sizeof(AEArray<unsigned int>));
    newhighsys->size = newsize;
    newhighsys->size2 = newsize;
    newhighsys->data = reinterpret_cast<unsigned int*>(AbyssEngine::memory_allocate(sizeof(unsigned int) * newsize));
    memcpy(newhighsys->data, oldhighsys->data, sizeof(unsigned int) * old_size);
    for (int i = 0; i < static_cast<int>(created_items.size()); i++)
        newhighsys->data[old_size + i] = created_items[i].item.m_nHighestPriceSystemId;
    globals_status->m_pItemHighestPricesSystem = newhighsys;
    AbyssEngine::memory_free(oldhighsys->data);
    AbyssEngine::memory_free(oldhighsys);
}