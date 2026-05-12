#ifndef HOOKS_H
#define HOOKS_H
#include <Game/structs.h>
#include "offset.h"

class Hooks {
    private:
        using globals_init = uintptr_t (__stdcall*)(uintptr_t, uintptr_t, uintptr_t);
        inline static globals_init oldglobals_init = nullptr;
        static uintptr_t __stdcall globals_init_hook(uintptr_t a, uintptr_t b, uintptr_t c);
        
        using fileread_loadstationbinaryfromid = uintptr_t(__stdcall*)(const uint16_t*);
        inline static fileread_loadstationbinaryfromid old_filereadloadstationbinaryfromid = nullptr;
        static uintptr_t __stdcall fileread_loadstationbinaryfromid_hook(const uint16_t* id);

        using fileread_loadstationbinary = uintptr_t(__stdcall*)(SingleSystem*);
        inline static fileread_loadstationbinary old_filereadloadstationbinary = nullptr;
        static uintptr_t __stdcall fileread_loadstationbinary_hook(SingleSystem *system);

        using standing_isenemy = bool (__fastcall*)(uintptr_t, int);
        inline static standing_isenemy old_standingisenemy = nullptr;
        static bool __fastcall standing_isenemy_hook(uintptr_t standing, int race);

        using abyssengine_paintcanvas_setcolor = void (__fastcall*)(uintptr_t);
        inline static abyssengine_paintcanvas_setcolor old_abyssenginepaintcanvassetcolor = nullptr;
        static void __fastcall abyssengine_paintcanvas_setcolor_hook(uintptr_t paintcanvas);

        using gametext_gettext = AEString* (__fastcall*)(void);
        inline static gametext_gettext old_gametextgettext = nullptr;
        static AEString* __fastcall gametext_gettext_hook(void);

        using recordhandler_recordstorewrite = int (__stdcall*)(uintptr_t, int);
        inline static recordhandler_recordstorewrite old_recordhandlerrecordstorewrite = nullptr;
        static int __stdcall recordhandler_recordstorewrite_hook(uintptr_t a, int b);

        using level_creategun = int (__stdcall*)(int, int, int, int, int, int, int, int, int);
        inline static level_creategun old_levelcreategun = nullptr;
        static int __stdcall level_creategun_hook(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);

        using level_createship = int* (__stdcall*)(uintptr_t*, int, int, int, uintptr_t*, int);
        inline static level_createship old_levelcreateship = nullptr;
        static int* __stdcall level_createship_hook(uintptr_t *a1, int a2, int a3, int a4, uintptr_t *a5, int a6);

        using imagefactory_drawchar = void (__stdcall*)(int, int, int);
        inline static imagefactory_drawchar old_imagefactorydrawchar = nullptr;
        static int __stdcall imagefactory_drawchar_hook(int a2, int a3, int a4);

        using imagepart_draw = void (__stdcall*)(unsigned int, int, int, int, int, int, int, int);
        inline static imagepart_draw old_imagepartdraw = nullptr;
        static void __stdcall imagepart_draw_hook(unsigned int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

        using abyssengine_paintcanvas_drawimage2d = void (__stdcall*)(int, unsigned int, int, int, int);
        inline static abyssengine_paintcanvas_drawimage2d old_abyssenginepaintcanvasdrawimage2d = nullptr;
        static void __stdcall abyssengine_paintcanvas_drawimage2d_hook(int a1, unsigned int a2, int a3, int a4, int a5);

        using radiomessage_radiomessage = int (__stdcall*)(int, int);
        inline static radiomessage_radiomessage old_radiomessageradiomessage = nullptr;
        static int __stdcall radiomessage_radiomessage_hook(int a4, int a5);

        using imagefactory_loadchar = int (__stdcall*)(int*);
        inline static imagefactory_loadchar old_imagefactoryloadchar = nullptr;
        static int __stdcall imagefactory_loadchar_hook(int *a1);

        using dialoguewindow_dialoguewindow = int (__stdcall*)(int*, int*, int*, int);
        inline static dialoguewindow_dialoguewindow old_dialoguewindowdialoguewindow = nullptr;
        static int __stdcall dialoguewindow_dialoguewindow_hook(int *a1, int *a2, int *a3, int a4);

        using dialoguewindow_loadcontent = int (__stdcall*)(int*);
        inline static dialoguewindow_loadcontent old_dialoguewindowloadcontent = nullptr;
        static int __stdcall dialoguewindow_loadcontent_hook(int *a1);

        using dialoguewindow_set = int (__stdcall*)(int a4);
        inline static dialoguewindow_set old_dialoguewindowset = nullptr;
        static int __stdcall dialoguewindow_set_hook(int a4);

        using mgame_togglepause = int (__cdecl*)();
        inline static mgame_togglepause old_mgametogglepause = nullptr;
        static int __cdecl mgame_togglepause_hook();

        using cutscene_cutscene = int (__cdecl*)();
        inline static cutscene_cutscene old_cutscenecutscene = nullptr;
        static int __cdecl cutscene_cutscene_hook();

        using targetfollowcamera_setposition = float* (__cdecl*)();
        inline static targetfollowcamera_setposition old_targetfollowcamerasetposition = nullptr;
        static float* __cdecl targetfollowcamera_setposition_hook();

        using levelscript_process = char (__thiscall*)(LevelScript*, int);
        inline static levelscript_process old_levelscriptprocess = nullptr;
        static char __thiscall levelscript_process_hook(LevelScript *a1, int a2);

        using route_route = int (__stdcall*)(int, int);
        inline static route_route old_routeroute = nullptr;
        static int __stdcall route_route_hook(int a2, int a3);

        using waypoint_waypoint = int (__stdcall*)(int, int, int, int*);
        inline static waypoint_waypoint old_waypointwaypoint = nullptr;
        static int __stdcall waypoint_waypoint_hook(int a2, int a3, int a4, int *a5);

        using radar_draw = void (__stdcall*)(int*, float*, int*, int);
        inline static radar_draw old_radardraw = nullptr;
        static void __stdcall radar_draw_hook(int* a1, float* a2, int* a3, int a4);

        using route_getwaypoint = int (__cdecl*)();
        inline static route_getwaypoint old_routegetwaypoint = nullptr;
        static int __cdecl route_getwaypoint_hook();

        using playerfighter_render = float* (__thiscall*)(unsigned int*);
        inline static playerfighter_render old_playerfighterrender = nullptr;
        static float* __thiscall playerfighter_render_hook(unsigned int *a1);

        using level_assignguns = int* (__stdcall*)(int*);
        inline static level_assignguns old_levelassignguns = nullptr;
        static int* __stdcall level_assignguns_hook(int *a1);

        using kiplayer_addgun = void (__stdcall*)(int);
        inline static kiplayer_addgun old_kiplayeraddgun = nullptr;
        static void __stdcall kiplayer_addgun_hook(int a3);

        using globals_getshipgroup = int* (__stdcall*)(int, int, char);
        inline static globals_getshipgroup old_globalsgetshipgroup = nullptr;
        static int* __stdcall globals_getshipgroup_hook(int a1, int a2, char a3);

        using level_update = void (__stdcall*)(int, int, int);
        inline static level_update old_levelupdate = nullptr;
        static void __stdcall level_update_hook(int a2, int a3, int a4);
        
        using level_createmission = void (__thiscall*)(void*);
        inline static level_createmission old_levelcreatemission = nullptr;
        static void __thiscall level_createmission_hook(void *a1);

        using modmainmenu_onrender2d = void (__thiscall*)(int*);
        inline static modmainmenu_onrender2d old_modmainmenuonrender2d = nullptr;
        static void __thiscall modmainmenu_onrender2d_hook(int *a1);

        using level_createstaticobject = int* (__stdcall*)(int, int*, int);
        inline static level_createstaticobject old_levelcreatestaticobject = nullptr;
        static int* __stdcall level_createstaticobject_hook(int a1, int *a2, int a3);

        using mgame_onrender2d = void (__thiscall*)(int*);
        inline static mgame_onrender2d old_mgameonrender2d = nullptr;
        static void __thiscall mgame_onrender2d_hook(int *a1);

        using level_createspace = unsigned int (__stdcall*)(int*);
        inline static level_createspace old_levelcreatespace = nullptr;
        static unsigned int __stdcall level_createspace_hook(int *a1);

        using status_nextcampaignmission = void (__thiscall*)(int*);
        inline static status_nextcampaignmission old_statusnextcampaignmission = nullptr;
        static void __thiscall status_nextcampaignmission_hook(int *a1);

        using starmap_init = int (__fastcall*)(int, int, char, int, char);
        inline static starmap_init old_starmapinit = nullptr;
        static int __fastcall starmap_init_hook(int a1, int a2, char a3, int a4, char a5);

        using starmap_starmap = int (__stdcall*)(int, int, int, int);
        inline static starmap_starmap old_starmapstarmap = nullptr;
        static int __stdcall starmap_starmap_hook(int a2, int a3, int a4, int a5);

        using status_getcampaignmission = int (__cdecl*)();
        inline static status_getcampaignmission old_statusgetcampaignmission = nullptr;
        static int __cdecl status_getcampaignmission_hook();

        using status_getfreelancemission = int (__cdecl*)();
        inline static status_getfreelancemission old_statusgetfreelancemission = nullptr;
        static int __cdecl status_getfreelancemission_hook();

        using starmap_dtor = int (__cdecl*)();
        inline static starmap_dtor old_starmapdtor = nullptr;
        static int __cdecl starmap_dtor_hook();

        using starmap_ontouchend = char (__fastcall*)(int, int*, int);
        inline static starmap_ontouchend old_starmaptouchend = nullptr;
        static char __fastcall starmap_ontouchend_hook(int a1, int *a2, int a3);

        using modstation_leavestation = char (__cdecl*)();
        inline static modstation_leavestation old_modstationleavestation = nullptr;
        static char __cdecl modstation_leavestation_hook();

        using modstation_onupdate = int (__thiscall*)(ModStation*);
        inline static modstation_onupdate old_modstationonupdate = nullptr;
        static int __thiscall modstation_onupdate_hook(ModStation *a1);

        using level_createcampaignmission = int (__thiscall*)(void*);
        inline static level_createcampaignmission old_levelcreatecampaignmission = nullptr;
        static int __thiscall level_createcampaignmission_hook(void *a1);

        using mgame_onupdate = void (__thiscall*)(MGame*);
        inline static mgame_onupdate old_mgameonupdate = nullptr;
        static void __thiscall mgame_onupdate_hook(MGame *a1);

        using dialoguewindow_dtor = int* (__thiscall*)(void*, int *a2);
        inline static dialoguewindow_dtor old_dialoguewindowdtor = nullptr;
        static int* __thiscall dialoguewindow_dtor_hook(void *a1, int *a2);

        using starmap_depart = int (__stdcall*)(int);
        inline static starmap_depart old_starmapdepart = nullptr;
        static int __stdcall starmap_depart_hook(int a2);

        static void injectsystemsandstations(void);
        static void injectitems(void);
    public:
        static void init(void);
};

#endif