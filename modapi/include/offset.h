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
        static constexpr uintptr_t GLOBALS_GETDIALOGUESOUNDID = 0x44DAC3;
        static constexpr uintptr_t GLOBALS_GETSHIPGROUP = 0x44CF2E;
        static constexpr uintptr_t GLOBALS_FONT = 0x60ADCC;
        static constexpr uintptr_t GLOBALS_W = 0x60AF00;
        static constexpr uintptr_t GLOBALS_H = 0x60AE80;

        static constexpr uintptr_t CAMPAIGN_BRIEFING_LENGTHS = 0x5311E8;
        static constexpr uintptr_t CAMPAIGN_SUCCESS_LENGTHS = 0x5314F0;
       
        static constexpr uintptr_t FILEREAD_LOADSTATIONBINARYFROMID = 0x408880;
        static constexpr uintptr_t FILEREAD_LOADSTATIONBIRARY = 0x408C4B;
        
        static constexpr uintptr_t STANDING_ISENEMY = 0x4CE34B;
        
        static constexpr uintptr_t ABYSSENGINE_PAINTCANVAS_SETCOLOR = 0x4EC3E0;
        static constexpr uintptr_t ABYSSENGINE_PAINTCANVAS_DRAWIMAGE2D = 0x4E85F0;
        static constexpr uintptr_t ABYSSENGINE_PAINTCANVAS_DRAWIMAGE2D_2 = 0x4E8780;
        static constexpr uintptr_t ABYSSENGINE_PAINTCANVAS_TRANSFORMGETLOCAL = 0x4EAAE0;
        static constexpr uintptr_t ABYSSENGINE_PAINTCANVAS_GETTEXTWIDTH = 0x4EB440;
        static constexpr uintptr_t ABYSSENGINE_PAINTCANVAS_DRAWSTRING = 0x4E8130;
        static constexpr uintptr_t ABYSSENGINE_ENGINE_ENGINE = 0x4E77B0;
        
        static constexpr uintptr_t GAMETEXT_GETTEXT = 0x4F38B0;
        
        static constexpr uintptr_t RECORDHANDLER_RECORDSTOREWRITE = 0x4BF40C;
        
        static constexpr uintptr_t LEVEL_CREATEGUN = 0x4695CB;
        static constexpr uintptr_t LEVEL_CREATESHIP = 0x474B42;
        static constexpr uintptr_t LEVEL_CREATEASTEROIDS = 0x468871;
        static constexpr uintptr_t LEVEL_ASSIGNGUNS = 0x4736D7;
        static constexpr uintptr_t LEVEL_UPDATE = 0x4793F0;
        static constexpr uintptr_t LEVEL_RENDER = 0x47896B;
        static constexpr uintptr_t LEVEL_CREATEMISSION = 0x46A0E9;
        static constexpr uintptr_t LEVEL_CREATESTATICOBJECT = 0x474246;
        static constexpr uintptr_t LEVEL_CREATESPACE = 0x467D5A;
        static constexpr uintptr_t LEVEL_INIT = 0x467061;
        static constexpr uintptr_t LEVEL_CREATECAMPAIGNMISSION = 0x46D9E9;
        static constexpr uintptr_t LEVEL_CREATERADIOMESSAGE = 0x479678;
        
        static constexpr uintptr_t IMAGEFACTORY_DRAWCHAR = 0x45F4C5;
        static constexpr uintptr_t IMAGEFACTORY_LOADCHAR = 0x45F31A;

        static constexpr uintptr_t IMAGEPART_DRAW = 0x4E8990;

        static constexpr uintptr_t RADIOMESSAGE_RADIOMESSAGE = 0x4BCD33;

        static constexpr uintptr_t DIALOGUEWINDOW_DIALOGUEWINDOW = 0x405E2D;
        static constexpr uintptr_t DIALOGUEWINDOW_LOADCONTENT = 0x40645E;
        static constexpr uintptr_t DIALOGUEWINDOW_SET = 0x406095;
        static constexpr uintptr_t DIALOGUEWINDOW_DTOR = 0x490D7C;

        static constexpr uintptr_t MGAME_TOGGLEPAUSE = 0x497564;
        static constexpr uintptr_t MGAME_ONUPDATE = 0x492BB4;
        static constexpr uintptr_t MGAME_ONRENDER2D = 0x495537;

        static constexpr uintptr_t CUTSCENE_CUTSCENE = 0x404971;
        static constexpr uintptr_t CUTSCENE_RENDER3D = 0x405CDC;

        static constexpr uintptr_t TARGETFOLLOWCAMERA_SETPOSITION = 0x4DBBDB;
        static constexpr uintptr_t TARGETFOLLOWCAMERA_TRANSLATE = 0x4DBBFF;

        static constexpr uintptr_t LEVELSCRIPT_PROCESS = 0x47AA08;

        static constexpr uintptr_t LODMANAGER_FORCEUPDATE = 0x483B8F;
        static constexpr uintptr_t LODMANAGER_ADDOBJECT = 0x483AEA;

        static constexpr uintptr_t ROUTE_ROUTE = 0x4C2311;
        static constexpr uintptr_t ROUTE_GETWAYPOINT = 0x4C245F;

        static constexpr uintptr_t WAYPOINT_WAYPOINT = 0x4DE032;

        static constexpr uintptr_t RADAR_DRAW = 0x4BA052;

        static constexpr uintptr_t KIPLAYER_SETROUTE = 0x4605E7;
        static constexpr uintptr_t KIPLAYER_ADDGUN = 0x4A9BD4;

        static constexpr uintptr_t SHIP_HASJUMPDRIVE = 0x004C2F11;

        static constexpr uintptr_t PLAYEREGO_SETPOSITION = 0x4AC590;
        static constexpr uintptr_t PLAYEREGO_TOGGLECLOAKING = 0x004ABA3B;
        static constexpr uintptr_t PLAYEREGO_UPDATE = 0x4AD64F;

        static constexpr uintptr_t PLAYERASTEROID_PLAYERASTEROID = 0x4AA595;

        static constexpr uintptr_t PLAYERFIGHTER_PLAYERFIGHTER = 0x4B19F3;
        static constexpr uintptr_t PLAYERFIGHTER_RENDER = 0x4B4F27;
        static constexpr uintptr_t PLAYERFIGHTER_UPDATE = 0x4B2497;
        
        static constexpr uintptr_t PLAYER_PLAYER = 0x4A9088;

        static constexpr uintptr_t AEGEOMETRY_AEGEOMETRY = 0x401944;
        static constexpr uintptr_t AEGEOMETRY_SETLODMESHES = 0x402195;
        static constexpr uintptr_t AEGEOMETRY_RENDER = 0x401F31;

        static constexpr uintptr_t GUN_GUN = 0x45105D;
        static constexpr uintptr_t GUN_SHOOTAT = 0x451556;
        static constexpr uintptr_t GUN_SETINDEX = 0x45139F;
        static constexpr uintptr_t GUN_SETENEMY = 0x4514FC;

        static constexpr uintptr_t PARTICLESYSTEMMANAGER_UPDATE = 0x4A6A14;

        static constexpr uintptr_t MODMAINMENU_ONRENDER2D = 0x499DCD;

        static constexpr uintptr_t STATUS_NEXTCAMPAIGNMISSION = 0x4D605F;
        static constexpr uintptr_t STATUS_SETCAMPAIGNMISSION = 0x4D5FAD;
        static constexpr uintptr_t STATUS_GETCAMPAIGNMISSION = 0x4D5F9C;
        static constexpr uintptr_t STATUS_GETFREELANCEMISSION = 0x4D5F85;

        static constexpr uintptr_t MISSION_MISSION = 0x4986DE;

        static constexpr uintptr_t STARMAP_INIT = 0x4CEAFE;
        static constexpr uintptr_t STARMAP_STARMAP = 0x4CE5D0;
        static constexpr uintptr_t STARMAP_DTOR = 0x4CE9E3;
        static constexpr uintptr_t STARMAP_ONTOUCHEND = 0x4D332E;
        static constexpr uintptr_t STARMAP_DEPART = 0x4CFE97;

        static constexpr uintptr_t AEGEOMETRY_DTOR = 0x402A05;

        static constexpr uintptr_t MODSTATION_LEAVESTATION = 0x4A0367;
        static constexpr uintptr_t MODSTATION_ONUPDATE = 0x49E1BD;
};
#endif