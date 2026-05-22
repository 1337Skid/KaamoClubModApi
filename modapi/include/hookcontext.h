#ifndef HOOKCONTEXT_H
#define HOOKCONTEXT_H
#include <string>
#include <sol/sol.hpp>

class HookContext {
    public:
        bool call_original = false;
        void call() { call_original = true; }
};

class MissionContext : public HookContext {
    public:
        void createfighter(int meshid, int faction);
        void createstaticobject(int type, float x, float y, float z);
};

class GlobalsInitContext : public HookContext {
    public:
        int createship(const std::string& name, const std::string& description, sol::table shipinfo, int diffuse, int normal, int material, int lod0, int lod1, int lod2);
};

#endif