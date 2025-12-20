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

struct Galaxy {
    uint8_t* stations;
    AEArray<SingleSystem*>* systems;
};

#endif