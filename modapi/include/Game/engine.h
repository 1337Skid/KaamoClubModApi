#ifndef ENGINE_H
#define ENGINE_H
#include <Game/structs.h>
#include "offset.h"

class Engine {
    private:
        static inline Globals_appManager **globals_appmanager = 0;
    public:
        static void init(void);
        static void setcurrentapplicationmodule(int id);
};
#endif