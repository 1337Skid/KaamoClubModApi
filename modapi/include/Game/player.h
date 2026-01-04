#ifndef PLAYER_H
#define PLAYER_H
#include <Game/structs.h>

class Player {
    private:
        static inline Globals_status* globals_status = 0;
        static inline Globals_appManager* globals_appmanager = 0;
    public:
        static void init(void);
        static int getmoney(void);
        static void setmoney(int value);
        static int getmaxcargo(void);
        static void setmaxcargo(int value);
        static int getcargo(void);
        static void setcargo(int value);
        static int getshiparmor(void);
        static void setshiparmor(int value);
        static int getmaxshiphealth(void);
        static void setmaxshiphealth(int value);
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
};
#endif