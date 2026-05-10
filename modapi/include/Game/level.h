#ifndef LEVEL_H
#define LEVEL_H
#include <Game/structs.h>
class Level {
    private:
        static inline Globals_appManager** globals_appmanager = 0;
        static inline Globals_status** globals_status = 0;
        static inline lua_State* lstate = nullptr;
        struct CustomRadioMsg {
            int race;
            int hair;
            int eyes;
            int mouth;
            int armor;
            std::wstring name;
            std::wstring content;
        };
        struct CustomDialogueMsg {
            int race;
            int hair;
            int eyes;
            int mouth;
            int armor;
            int isplayer;
            std::wstring name;
            std::wstring content;
        };
        struct CustomCutscenePoint {
            float x; 
            float y;
            float z;
            float duration;
            float shakeamount;
            int shakefrequency;
        };
        struct CustomPlayerFighter {
            int meshid;
            int faction;
        };
    public:
        static inline std::vector<CustomRadioMsg> created_radiomessages;
        static inline std::vector<CustomDialogueMsg> created_dialoguemessages;
        static inline std::vector<CustomCutscenePoint> created_cutscenepts;
        static inline std::vector<CustomPlayerFighter> created_playerfighters;
        static inline int current_dialogue_id = 0;
        static inline int created_cutscene = 0;
        static inline float cutscene_timer = 0.0f;
        static inline int cutscene_pointid = 0;
        static inline bool cutscene_anchored = false;
        static inline float cutscene_anchorX = 0.0f;
        static inline float cutscene_anchorY = 0.0f;
        static inline float cutscene_anchorZ = 0.0f;
        static inline uintptr_t cutscene_originaltarget = 0;
        static void init(lua_State* lua_state);
        static void createradiomessage(const std::string& name, const std::string& content, sol::table imageinfo);
        static void createdialoguewindow(sol::table dialogueinfo);
        static void createcutscene(sol::table camerapoints);
        static int* createroute(sol::table pospoints);
        static sol::table getentities(void);
        static void createasteroid(float x, float y, float z, float scale, int meshid);
        static void createfighter(int meshid, int faction);
};
#endif