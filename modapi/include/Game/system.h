#ifndef SYSTEM_H
#define SYSTEM_H
#include <Game/structs.h>

class System {
    private:
        static inline Globals_status **globals_status = 0;
        static inline lua_State* lstate = nullptr;
        SingleSystem *_ptr = nullptr;
    public:
        System();
        System(SingleSystem *ptr);
        ~System();

        static inline std::vector<SingleSystem> created_systems;
        SingleSystem *getstruct(void) const;
        static void init(lua_State *lua_state);
        int getid(void);
        void setid(int value);
        int getrisklevel(void);
        void setrisklevel(int value);
        int getfaction(void);
        void setfaction(int value);
        int getjumpgatestationid(void);
        void setjumpgatestationid(int value);
        int getmapcoordinatex(void);
        void setmapcoordinatex(int value);
        int getmapcoordinatey(void);
        void setmapcoordinatey(int value);
        int getmapcoordinatez(void);
        void setmapcoordinatez(int value);
        int gettextureid(void);
        void settextureid(int value);
        std::string getname(void);
        void setname(std::string value);

        static int create(const std::string& str, int x, int y, int z, int faction, int risk, int textureid, const std::vector<int>& linkedids, int jumpgatestationid);
        static bool isvisible(int systemid);
        static void setvisible(int systemid, bool visible);
        static SingleSystem *getsystembyid(int id);
        sol::table getstationsid();
};

#endif