#ifndef HOOKCONTEXT_H
#define HOOKCONTEXT_H
#include <string>
#include <sol/sol.hpp>
#include <Game/touchbutton.h>
#include <Game/choicewindow.h>

class HookContext {
    public:
        bool call_original = false;
        void call() { call_original = true; }
};

class MissionContext : public HookContext {
    public:
        int *createfighter(int meshid, int faction);
        int *createstaticobject(int type, float x, float y, float z);
};

class GlobalsInitContext : public HookContext {
    public:
        int createship(const std::string& name, const std::string& description, sol::table shipinfo, int diffuse, int normal, int material, int lod0, int lod1, int lod2);
        TouchButton createtouchbutton(const std::string &text, const std::string &subtext, int x, int y, int textcolor, int state, sol::main_protected_function onclick);
        ChoiceWindow createchoicewindow(const std::string &title, const std::string &description, bool show_buttons, sol::main_protected_function onclick_yes, sol::main_protected_function onclick_no);
};

class Render2DContext : public HookContext {
    public:
        void drawstring(const std::string& text, int x, int y, int r, int g, int b, int a);
        void drawimage2d(unsigned int id, int x, int y, int r, int g, int b, int a);
};

class ModStationWindowContext : public Render2DContext {
    // TODO: maybe we give to the user a window_ptr and this window_ptr could be useful to edit the whole UI?
    // I made this context because the window hooks are special they are called after the real function being called
};

class GetTextContext : public HookContext {
    public:
        uintptr_t returnaddr = 0;
        int id = 0;
        bool overridden = false;
        std::wstring overriddentext;
        
        void overridetext(const std::string &text);
};

class SetColorContext : public HookContext {
    public:
        uintptr_t returnaddr = 0;
        unsigned int hexcolor = 0;
        bool overridden = false;

        void overridecolor(int r, int g, int b, int a);
};

#endif