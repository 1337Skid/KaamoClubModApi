#ifndef PLAYER_H
#define PLAYER_H
#include <Game/structs.h>

class Player {
    private:
        static inline Globals_status** globals_status = 0;
        static inline Globals_appManager** globals_appmanager = 0;
        static inline lua_State* lstate = nullptr;
    public:
        static void init(lua_State* lua_state);
        static int getmoney(void);
        static void setmoney(int value);
        static void setshipinfo(sol::table shipinfo);
        static int getenemieskilled(void);
        static void setenemieskilled(int value);
        static int getlevel(void);
        static void setlevel(int value);
        static int getvisitedstations(void);
        static void setvisitedstations(int value);
        static int getjumpgateusedcount(void);
        static void setjumpgateusedcount(int value);
        static int getcargosalvagedcount(void);
        static void setcargosalvagedcount(int value);
        static int getasteroidsdestroyedcount(void);
        static void setasteroidsdestroyedcount(int value);

        static bool hasshiparmor(void);
        static bool isdocked(void);
        static sol::table getshipinfo(void);
        static bool hasjumpdrive(void);
        static void togglecloaking(void);
        static void setposition(float x, float y, float z);
};
#endif