#ifndef STRUCTS_H
#define STRUCTS_H

struct AEString {
    wchar_t* text; 
    uint32_t size; 
};

template<typename T>
struct AEArray {
    uint32_t size; 
    T* data;
    uint32_t size2; 
};

struct Vector3Int {
    int32_t x;
    int32_t y;
    int32_t z; 
};

struct SingleSystem {
    uint8_t unk0[0xC];
    AEString name;
    uint32_t id;
    uint32_t risk;
    uint32_t faction;
    Vector3Int pos;
    uint32_t jumpgate_station_id;
    uint32_t texture_id;
    AEArray<uint32_t>* station_ids;
    uint8_t unk1[0x4];
    AEArray<uint32_t>* linked_system_ids;
    bool starts_unlocked;
};

struct SingleStation {
    AEString name;
    size_t id;
    size_t systemid;
    size_t unk0;
    size_t textureid;
    size_t unk1;
    size_t techlevel;
    std::array<uint8_t, 16> unk2;
};

struct SingleItem {
   int m_nID;
   int m_nType;
   int m_nSubType;
   int m_nTechLevel;
   int m_nLowestPriceSystemId;
   int m_nHighestPriceSystemId;
   int m_nPrice;
   int m_nOccurance;
   int m_nMinPrice;
   int maxPrice;
   int amount;
   int stationAmount;
   int blueprintAmount;
};

struct Galaxy {
    uint8_t* stations;
    AEArray<SingleSystem*>* systems;
};

struct ShipItems {
    int m_nPrimary;
    int m_nSecondary;
    int m_nTurret;
    int m_nEquipment;
};

struct ShipInfo {
    int m_nShipId;
    int m_nMaxShipHealth;
    int m_nBasePrice;
    int m_nMaxCapacity;
    int m_nCurrentCapacity;
    int m_nPrice;
    float m_fControlMultiplier;
    int field_1C;
    int m_nArmor;
    int field_24;
    int m_nBonusCapacity;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    float m_fPower; //48
	bool m_bRepairBot;
	bool m_bJumpDrive;
	bool m_bCloak;
	short Race;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int* m_pShipInformation;
    AEArray<SingleItem>* m_pShipEquipment;
    AEArray<SingleItem>* m_pShipItems;
};

struct Globals_appManager {
    int* m_pPaintCanvas; //PaintCanvas*
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int* m_pGlobalUnkStruct; //GlobalUnkStruct
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int* field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int m_nCurrentModule;
    int* m_pGlobalUnkStruct2; //GlobalUnkStruct2
};

struct Globals_status {
    int* field_0;
    int* field_4;
    int field_8;
    int* field_C; //custom array, field_C -> 0 = count, 4 = pointer
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    AEArray<unsigned int>* m_pSystemMapLinesInfo;
    int* field_28;
    int* field_2C;
    int* field_30;
    int* field_34;
    int* field_38;
    int* field_3C;
    int* field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int* field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    AEArray<SingleItem>* m_pShipItems;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int m_nAsteroidsDestroyed;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    ShipInfo* m_pShipInfo;
    int* field_158;
    int* field_15C;
    int* m_pStationInfo;
    int field_164;
    int* m_pCurrentPlayerSystemInfo;
    int field_16C;
    int field_170;
    int m_nMoney;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int m_nEnemiesKilled;
    int m_nCompletedSideMissions;
    int m_nPlayerLevel;
    int field_194;
    int m_nVisitedStations;
    int field_19C;
    int field_1A0;
    int field_1A4;
    int m_nCargoSalvaged;
    int field_1AC;
    int m_nCurrentCampaignMission;
    int field_1B4;
    int field_1B8;
    int field_1BC;
    int field_1C0;
    int field_1C4;
    int field_1C8;
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    int field_1E8;
    int field_1EC;
    int field_1F0;
    int field_1F4;
};

#endif