#ifndef OFFSET_H
#define OFFSET_H
class Offset {
    public:
        static constexpr uintptr_t GLOBALS_STATUS = 0x60AD6C;
        static constexpr uintptr_t GLOBALS_APPMANAGER = 0x60AEFC;
        static constexpr uintptr_t GLOBALS_CANVAS = 0x60AE68;
        static constexpr uintptr_t GLOBALS_INIT_ADDR = 0x0044B20C;
        static constexpr uintptr_t GLOBALS_GALAXY = 0x60AF3C;
        static constexpr uintptr_t GLOBALS_ITEMS = 0x60AEA4;
       
        static constexpr uintptr_t FILEREAD_LOADSTATIONBINARYFROMID = 0x408880;
        static constexpr uintptr_t FILEREAD_LOADSTATIONBIRARY = 0x408C4B;
        
        static constexpr uintptr_t STANDING_ISENEMY = 0x4CE34B;
        
        static constexpr uintptr_t ABYSSENGINE_PAINTCANVAS_SETCOLOR = 0x4EC3E0;
        
        static constexpr uintptr_t GAMETEXT_GETTEXT = 0x4F38B0;
        
        static constexpr uintptr_t RECORDHANDLER_RECORDSTOREWRITE = 0x4BF40C;
        
        static constexpr uintptr_t LEVEL_CREATEGUN = 0x4695CB;
        static constexpr uintptr_t LEVEL_CREATESHIP = 0x474B42;
        
        static constexpr uintptr_t IMAGEFACTORY_DRAWCHAR = 0x45F4C5;
};
#endif