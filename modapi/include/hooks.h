#ifndef HOOKS_H
#define HOOKS_H
#include <Game/structs.h>
#include "offset.h"

class Hooks {
    private:
        using globals_init = uintptr_t (__stdcall*)(uintptr_t, uintptr_t, uintptr_t);
        static globals_init oldglobals_init;
        static uintptr_t __stdcall globals_init_hook(uintptr_t a, uintptr_t b, uintptr_t c);
        
        using fileread_loadstationbinaryfromid = uintptr_t(__stdcall*)(const uint16_t*);
        static fileread_loadstationbinaryfromid old_filereadloadstationbinaryfromid;
        static uintptr_t __stdcall fileread_loadstationbinaryfromid_hook(const uint16_t* id);

        using fileread_loadstationbinary = uintptr_t(__stdcall*)(SingleSystem*);
        static fileread_loadstationbinary old_filereadloadstationbinary;
        static uintptr_t __stdcall fileread_loadstationbinary_hook(SingleSystem *system);

        using standing_isenemy = bool (__fastcall*)(uintptr_t, int);
        static standing_isenemy old_standingisenemy;
        static bool __fastcall standing_isenemy_hook(uintptr_t standing, int race);

        using abyssengine_paintcanvas_setcolor = void (__fastcall*)(uintptr_t);
        static abyssengine_paintcanvas_setcolor old_abyssenginepaintcanvassetcolor;
        static void __fastcall abyssengine_paintcanvas_setcolor_hook(uintptr_t paintcanvas);

        using gametext_gettext = AEString* (__fastcall*)(void);
        static gametext_gettext old_gametextgettext;
        static AEString* __fastcall gametext_gettext_hook(void);

        using recordhandler_recordstorewrite = int (__stdcall*)(uintptr_t, int);
        static recordhandler_recordstorewrite old_recordhandlerrecordstorewrite;
        static int __stdcall recordhandler_recordstorewrite_hook(uintptr_t a, int b);

        using level_creategun = int (__stdcall*)(int, int, int, int, int, int, int, int, int);
        static level_creategun old_levelcreategun;
        static int __stdcall level_creategun_hook(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);

        using level_createship = int* (__stdcall*)(uintptr_t*, int, int, int, uintptr_t*, int);
        static level_createship old_levelcreateship;
        static int* __stdcall level_createship_hook(uintptr_t *a1, int a2, int a3, int a4, uintptr_t *a5, int a6);

        using imagefactory_drawchar = void (__stdcall*)(int, int, int);
        static imagefactory_drawchar old_imagefactorydrawchar;
        static int __stdcall imagefactory_drawchar_hook(int a2, int a3, int a4);

        static void injectsystemsandstations(void);
        static void injectitems(void);
    public:
        static void init(void);
};

#endif