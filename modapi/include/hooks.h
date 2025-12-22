#ifndef HOOKS_H
#define HOOKS_H
#include <Game/structs.h>

class Hooks {
    private:
        static constexpr uintptr_t GLOBALS_INIT_ADDR = 0x0044B20C;
        static constexpr uintptr_t GLOBALS_GALAXY = 0x0060AF3C;
        static constexpr uintptr_t FILEREAD_LOADSTATIONBINARYFROMID = 0x408880;
        static constexpr uintptr_t FILEREAD_LOADSTATIONBIRARY = 0x408C4B;

        using globals_init = uintptr_t (__stdcall*)(uintptr_t, uintptr_t, uintptr_t);
        static globals_init oldglobals_init;
        static uintptr_t __stdcall globals_init_hook(uintptr_t a, uintptr_t b, uintptr_t c);
        
        using fileread_loadstationbinaryfromid = uintptr_t (__stdcall*)(const uint16_t*);
        static fileread_loadstationbinaryfromid old_filereadloadstationbinaryfromid;
        static uintptr_t __stdcall fileread_loadstationbinaryfromid_hook(const uint16_t* a);

        using fileread_loadstationbinary = uintptr_t (__stdcall*)(SingleSystem *system);
        static fileread_loadstationbinary old_filereadloadstationbinary;
        static uintptr_t __stdcall fileread_loadstationbinary_hook(SingleSystem *system);

        static void injectsystems(void);
    public:
        static void init(void);
};

#endif