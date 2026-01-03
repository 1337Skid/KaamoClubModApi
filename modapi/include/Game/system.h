#ifndef SYSTEM_H
#define SYSTEM_H
#include <Game/structs.h>

class System {
    private:
        static inline Globals_status* globals_status = 0;
    public:
        static inline std::vector<SingleSystem> created_systems;
        static void init(void);
        static int getid(void);
        static void setid(int value);
        static int getrisklevel(void);
        static void setrisklevel(int value);
        static int getfaction(void);
        static void setfaction(int value);
        static int getjumpgatestationid(void);
        static void setjumpgatestationid(int value);
        static int getmapcoordinatex(void);
        static void setmapcoordinatex(int value);
        static int getmapcoordinatey(void);
        static void setmapcoordinatey(int value);
        static int getmapcoordinatez(void);
        static void setmapcoordinatez(int value);
        static std::string getname(void);
        static void setname(std::string value);

        static int create(const std::string& str, int x, int y, int z, int faction, int risk, int textureid);
        static bool isvisible(int systemid);
        static void setvisible(int systemid, bool visible);
};

#endif