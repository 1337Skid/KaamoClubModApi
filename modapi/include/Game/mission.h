#ifndef MISSION_H
#define MISSION_H
#include <Game/structs.h>

class Mission {
    private:
        static inline Globals_status* globals_status = 0;
    public:
        static void init(void);
        static int getid(void);
        static void setid(int value);
        static int getcompletedsidemissions(void);
        static void setcompletedsidemissions(int value);
};
#endif