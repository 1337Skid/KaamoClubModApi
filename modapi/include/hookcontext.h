#ifndef HOOKCONTEXT_H
#define HOOKCONTEXT_H

class HookContext {
    public:
        bool call_original = false;
        void call() { call_original = true; }
};

class MissionContext : public HookContext {
    public:
        void createfighter(int meshid, int faction);
};

#endif