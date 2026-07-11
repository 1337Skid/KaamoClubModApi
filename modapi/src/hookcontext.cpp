#include "hookcontext.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <windows.h>
#include "modapi_utils.h"
#include <Game/structs.h>
#include <Game/level.h>
#include <Game/ship.h>
#include <Game/touchbutton.h>
#include <Game/choicewindow.h>
#include <windows.h>

int *MissionContext::createfighter(int meshid, int faction)
{
    return Level::createfighter(meshid, faction);
}

int *MissionContext::createstaticobject(int type, float x, float y, float z)
{
    return Level::createstaticobject(type, x, y, z);
}

int GlobalsInitContext::createship(const std::string& name, const std::string& description, sol::table shipinfo, int diffuse, int normal, int material, int lod0, int lod1, int lod2)
{
    return Ship::create(name, description, shipinfo, diffuse, normal, material, lod0, lod1, lod2);
}

TouchButton GlobalsInitContext::createtouchbutton(const std::string &text, const std::string &subtext, int x, int y, int textcolor, int state, sol::main_protected_function onclick)
{
    return TouchButton(text, subtext, x, y, textcolor, state, onclick);
}

void Render2DContext::drawstring(const std::string& text, int x, int y, int r, int g, int b, int a)
{
    Asset::setcolor(r, g, b, a);
    Asset::drawstring(text, x, y);
}

void Render2DContext::drawimage2d(unsigned int id, int x, int y, int r, int g, int b, int a)
{
    Asset::setcolor(r, g, b, a);
    Asset::drawimage2d(id, x, y);
}

ChoiceWindow GlobalsInitContext::createchoicewindow(const std::string &title, const std::string &description, bool show_buttons, sol::main_protected_function onclick_yes, sol::main_protected_function onclick_no)
{
    return ChoiceWindow(title, description, show_buttons, onclick_yes, onclick_no);
}

void GetTextContext::overridetext(const std::string &text)
{
    overridden = true;
}