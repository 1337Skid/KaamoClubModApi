#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "imguihandler.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "hookcontext.h"
#include "eventmanager.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/level.h>
#include <Game/structs.h>
#include <Game/kiplayer.h>
#include <Game/ship.h>
#include <Game/touchbutton.h>
#include <Game/engine.h>

void LuaManager::init()
{
    lua_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::os, sol::lib::io, sol::lib::coroutine);
}

void LuaManager::bind_api()
{
    lua_state.script("function wait(sec) coroutine.yield(sec) end");

    lua_state.new_usertype<HookContext>("HookContext",
        sol::no_constructor,
        "call", [](HookContext* self) {
            self->call();
        }
    );

    lua_state.new_usertype<MissionContext>("MissionContext",
        sol::no_constructor,
        sol::base_classes, sol::bases<HookContext>(),
        "CreateFighter", [](MissionContext* self, int meshid, int faction) -> KIPlayer {
            return KIPlayer(self->createfighter(meshid, faction));
        },
        "CreateStaticObject", [](MissionContext* self, int type, float x, float y, float z) -> KIPlayer {
            return KIPlayer(self->createstaticobject(type, x, y, z));
        }
    );

    lua_state.new_usertype<GlobalsInitContext>("GlobalsInitContext",
        sol::no_constructor,
        sol::base_classes, sol::bases<HookContext>(),
        "CreateShip", [](GlobalsInitContext* self, const std::string& name, const std::string& description, sol::table shipinfo, int diffuse, int normal, int material, int lod0, int lod1, int lod2) -> int {
            return self->createship(name, description, shipinfo, diffuse, normal, material, lod0, lod1, lod2);
        },
        "CreateTouchButton", [](GlobalsInitContext *self, const std::string &text, const std::string &subtext, int x, int y, int textcolor, int state, sol::main_protected_function onclick, sol::this_state s) -> TouchButton {
            sol::state_view lua(s);
            auto wrapper = [onclick](sol::variadic_args args) {
                sol::state_view lua(EventManager::lua_manager->getluastate());
                sol::thread thread = sol::thread::create(lua);
                sol::coroutine cor(thread.state(), onclick);
                std::vector<sol::object> vecargs;
                for (auto arg : args)
                    vecargs.push_back(arg);
                auto result = cor(sol::as_args(vecargs));
                EventManager::lua_manager->handle_coroutine(std::move(thread), std::move(cor), result, nullptr, std::move(vecargs));
            };
            sol::main_protected_function wrapped = sol::make_object(lua, wrapper).as<sol::main_protected_function>();
            return self->createtouchbutton(text, subtext, x, y, textcolor, state, wrapped);
        },
        "CreateChoiceWindow", [](GlobalsInitContext *self, const std::string &title, const std::string &description, bool show_buttons, sol::main_protected_function on_yes, sol::main_protected_function on_no, sol::this_state s) -> ChoiceWindow {
            sol::state_view lua(s);
            auto wrap_yes = [on_yes](sol::variadic_args args) {
                sol::state_view lua(EventManager::lua_manager->getluastate());
                sol::thread thread = sol::thread::create(lua);
                sol::coroutine cor(thread.state(), on_yes);
                std::vector<sol::object> vecargs;
                for (auto arg : args)
                    vecargs.push_back(arg);
                auto result = cor(sol::as_args(vecargs));
                EventManager::lua_manager->handle_coroutine(std::move(thread), std::move(cor), result, nullptr, std::move(vecargs));
            };
            auto wrap_no = [on_no](sol::variadic_args args) {
                sol::state_view lua(EventManager::lua_manager->getluastate());
                sol::thread thread = sol::thread::create(lua);
                sol::coroutine cor(thread.state(), on_no);
                std::vector<sol::object> vecargs;
                for (auto arg : args)
                    vecargs.push_back(arg);
                auto result = cor(sol::as_args(vecargs));
                EventManager::lua_manager->handle_coroutine(std::move(thread), std::move(cor), result, nullptr, std::move(vecargs));
            };
            sol::main_protected_function wrappedyes = sol::make_object(lua, wrap_yes).as<sol::main_protected_function>();
            sol::main_protected_function wrappedno = sol::make_object(lua, wrap_no).as<sol::main_protected_function>();
            return self->createchoicewindow(title, description, show_buttons, wrappedyes, wrappedno);
        }
    );

    lua_state.new_usertype<Render2DContext>("Render2DContext",
        sol::no_constructor,
        sol::base_classes, sol::bases<HookContext>(),
        "DrawString", [](Render2DContext* self, const std::string& text, int x, int y, int r, int g, int b, int a) {
            self->drawstring(text, x, y, r, g, b, a);
        },
        "DrawImage2D", [](Render2DContext* self, unsigned int id, int x, int y, int r, int g, int b, int a) {
            self->drawimage2d(id, x, y, r, g, b, a);
        }
    );
 
    lua_state.new_usertype<Player>("Player",
        sol::no_constructor,
        "money", sol::property(&Player::getmoney, &Player::setmoney),
        "enemieskilled", sol::property(&Player::getenemieskilled, &Player::setenemieskilled),
        "level", sol::property(&Player::getlevel, &Player::setlevel),
        "visitedstations", sol::property(&Player::getvisitedstations, &Player::setvisitedstations),
        "cargosalvagedcount", sol::property(&Player::getcargosalvagedcount, &Player::setcargosalvagedcount),
        "asteroidsdestroyedcount", sol::property(&Player::getasteroidsdestroyedcount, &Player::setasteroidsdestroyedcount),
        "HasShipArmor", [](Player& self) -> bool {
            return Player::hasshiparmor();
        },
        "IsDocked", [](Player& self) -> bool {
            return Player::isdocked();
        },
        "GetShipInfo", [](Player& self) -> sol::table {
            return Player::getshipinfo();
        },
        "SetShipInfo", [](Player& self, sol::table shipinfo) {
            Player::setshipinfo(shipinfo);
        },
        "HasJumpDrive", [](Player& self) -> bool {
            return Player::hasjumpdrive();
        },
        "ToggleCloaking", [](Player& self) {
            Player::togglecloaking();
        },
        "SetPosition", [](Player& self, float x, float y, float z) {
            Player::setposition(x, y, z);
        },
        "ResetGame", [](Player& self) {
            Player::resetgame();
        },
        "SetStation", [](Player& self, int id) {
            Player::setstation(id);
        }
    );

    lua_state.new_usertype<System>("System",
        sol::no_constructor,
        "id", sol::property(&System::getid, &System::setid),
        "name", sol::property(&System::getname, &System::setname),
        "risk", sol::property(&System::getrisklevel, &System::setrisklevel),
        "faction", sol::property(&System::getfaction, &System::setfaction),
        "jumpgatestationid", sol::property(&System::getjumpgatestationid, &System::setjumpgatestationid),
        "mapcoordinate_x", sol::property(&System::getmapcoordinatex, &System::setmapcoordinatex),
        "mapcoordinate_y", sol::property(&System::getmapcoordinatey, &System::setmapcoordinatey),
        "mapcoordinate_z", sol::property(&System::getmapcoordinatez, &System::setmapcoordinatez),
        "textureid", sol::property(&System::gettextureid, &System::settextureid),
        "Create", [](System& self, const std::string& str, int x, int y, int z, int faction, int risk, int textureid, int linkedsystemid) {
            return System::create(str, x, y, z, faction, risk, textureid, linkedsystemid);
        },
        "IsVisible", [](System& self, int systemid) {
            return System::isvisible(systemid);
        },
        "SetVisible", [](System& self, int systemid, bool visible) {
            return System::setvisible(systemid, visible);
        },
        "Get", [](System& self, int id) -> sol::optional<System> {
            SingleSystem *ptr = System::getsystembyid(id);
            if (ptr == nullptr)
                return sol::nullopt;
            return System(ptr);
        }
    );

    lua_state.new_usertype<Mission>("Mission",
        sol::no_constructor,
        "id", sol::property(&Mission::getid, &Mission::setid),
        "completedsidemissions", sol::property(&Mission::getcompletedsidemissions, &Mission::setcompletedsidemissions),
        "EnableValkyrie", [](Mission& self) {
            Mission::enablevalkyrie();
        },
        "NextCampaignMission", [](Mission& self) {
            Mission::nextcampaignmission();
        },
        "Create", [](Mission& self, int stationid, std::string description, int type) -> int {
            return Mission::create(stationid, description, type);
        },
        "Enable", [](Mission& self, int custom_missionid) {
            Mission::enable(custom_missionid);
        },
        "Disable", [](Mission& self, int custom_missionid) {
            Mission::disable(custom_missionid);
        }
    );

    lua_state.new_usertype<Station>("Station",
        sol::no_constructor,
        "id", sol::property(&Station::getid, &Station::setid),
        "name", sol::property(&Station::getname, &Station::setname),
        "level", sol::property(&Station::gettechlevel, &Station::settechlevel),
        "hangaritemscount", sol::property(&Station::gethangaritemscount, &Station::sethangaritemscount),
        "hangarshipscount", sol::property(&Station::gethangarshipscount, &Station::sethangarshipscount),
        "agentscount", sol::property(&Station::getagentscount, &Station::setagentscount),
        "IsVoid", [](Station& self) -> bool {
            return Station::isvoid();
        },
        "SetHangarShipInfo", [](Station& self, int id, sol::table shipinfo) {
            Station::sethangarshipinfo(id, shipinfo);
        },
        "Create", [](Station& self, const std::string& str, int techlevel, int textureid, int systemid) {
            return Station::create(str, techlevel, textureid, systemid);
        },
        "GetAgentName", [](Station& self, int id) {
            return Station::getagentname(id);
        },
        "GetAgentFaction", [](Station& self, int id) {
            return Station::getagentfaction(id);
        },
        "CreateAgent", [](Station& self, const std::string& name, int factiontype, int terranwoman, sol::table imageinfo, sol::table agentinfo) {
            Station::createagent(name, factiontype, terranwoman, imageinfo, agentinfo);
        },
        "RemoveHangarItem", [](Station& self, int id) {
            Station::removehangaritem(id);
        },
        "HasItemInHangar", [](Station& self, int id) -> bool {
            return Station::hasiteminhangar(id);
        },
        "AddHangarShip", [](Station&, int id) {
            Station::addhangarship(id);
        }
    );

    lua_state.new_usertype<Asset>("Asset",
        sol::no_constructor,
        "GetAssetFilePath", [](Asset& self, unsigned int id) -> std::string {
            return Asset::getassetfilepath(id);
        },
        "SetAssetFilePath", [](Asset& self, unsigned int id, const std::string filepath) {
            Asset::setassetfilepath(id, filepath);
        },
        "GetText", [](Asset& self, int id) -> std::string {
            return Asset::gettext(id);
        },
        "CreateTexture", [](Asset& self, const std::string& path) -> int {
            return Asset::createtexture(path);
        },
        "CreateMaterial", [](Asset& self, int diffuse, int normal, int shader) -> int {
            return Asset::creatematerial(diffuse, normal, shader);
        },
        "CreateMesh", [](Asset& self, const std::string& path, int materialid) -> int {
            return Asset::createmesh(path, materialid);
        },
        "Image2DCreate", [](Asset& self, uint16_t id) -> int {
            return Asset::image2dcreate(id);
        },
        "CreateSprite", [](Asset& self, int textureid, int regionid) -> int {
            return Asset::createsprite(textureid, regionid);
        }
    );

    lua_state.new_usertype<Item>("Item",
        sol::no_constructor,
        "Create", [](Item& self, const std::string& name, const std::string& description, sol::table iteminfo) -> int {
            return Item::create(name, description, iteminfo);
        },
        "UnlockBlueprint", [](Item& self, int id) {
            Item::unlockblueprint(id);
        },
        "LockBlueprint", [](Item& self, int id) {
            Item::lockblueprint(id);
        }
    );

    lua_state.new_usertype<ChoiceWindow>("ChoiceWindow",
        sol::no_constructor,
        "Show", [](ChoiceWindow& self) {
            self.show();
        },
        "Hide", [](ChoiceWindow& self) {
            self.hide();
        },
        sol::meta_function::to_string, &ChoiceWindow::tostring
    );

    lua_state.new_usertype<Level>("Level",
        sol::no_constructor,
        "CreateRadioMessage", [](Level& self, const std::string& name, const std::string& content, sol::table imageinfo)  {
            Level::createradiomessage(name, content, imageinfo);
        },
        "CreateDialogueWindow", [](Level& self, sol::table dialogueinfo) {
            Level::createdialoguewindow(dialogueinfo);
        },
        "CreateCutScene", [](Level& self, sol::table camerapoints) {
            Level::createcutscene(camerapoints);
        },
        "CreateRoute", [](Level& self, sol::table pospoints) -> LuaRoute {
            return LuaRoute{Level::createroute(pospoints)};
        },
        "GetEntities", [](Level& self) -> sol::table {
            return Level::getentities();
        },
        "CreateAsteroid", [](Level& self, float x, float y, float z, float scale, int meshid) {
            Level::createasteroid(x,y,z,scale,meshid); // TODO: return a playerasteroid/kiplayer?
        }
    );

    lua_state.new_usertype<Engine>("Engine",
        sol::no_constructor,
        "SetCurrentApplicationModule", [](Engine& self, int id)  {
            Engine::setcurrentapplicationmodule(id);
        }
    );

    lua_state.new_usertype<LuaRoute>("Route",
        sol::no_constructor,
        "IsValid", [](LuaRoute& self) -> bool {
            return self.ptr != nullptr;
        }
    );

    lua_state.new_usertype<KIPlayer>("KIPlayer",
        sol::no_constructor,
        "SetRoute", [](KIPlayer& self, LuaRoute& route) {
            self.setroute(route.ptr);
        },
        sol::meta_function::to_string, &KIPlayer::tostring
    );

    lua_state.new_usertype<TouchButton>("TouchButton",
        sol::no_constructor,
        "SetText", [](TouchButton& self, const std::string &text) {
            self.settext(text);
        },
        sol::meta_function::to_string, &TouchButton::tostring
    );
    
    lua_state.new_usertype<MemoryUtils>("MemoryUtils",
        sol::no_constructor,
        "PatchByte", [](MemoryUtils& self, unsigned int address, uint8_t value) {
            MemoryUtils::PatchByte(address, value);
        }
    );

    lua_state.new_usertype<ImGuiHandler>("ImGui",
        sol::no_constructor,
        "Text", [](ImGuiHandler&, const std::string& text) {
            ImGui::Text("%s", text.c_str());
        },
        "ColoredText", [](ImGuiHandler&, const std::string& text, float r, float g, float b, float a) {
            ImGui::TextColored(ImVec4(r, g, b, a), "%s", text.c_str());
        },
        "Button", [](ImGuiHandler&, const std::string& label) -> bool {
            return ImGui::Button(label.c_str());
        },
        "Checkbox", [](ImGuiHandler&, const std::string& text, bool value) -> bool {
            ImGui::Checkbox(text.c_str(), &value);
            return value;
        },
        "SliderFloat", [](ImGuiHandler&, const std::string& text, float value, float min, float max) -> float {
            ImGui::SliderFloat(text.c_str(), &value, min, max);
            return value;
        },
        "SliderInt", [](ImGuiHandler&, const std::string& text, int value, int min, int max) -> int {
            ImGui::SliderInt(text.c_str(), &value, min, max);
            return value;
        },
        "InputText", [](ImGuiHandler&, const std::string& text, const std::string& value) -> std::string {
            std::string buf = value;
            buf.resize(256);
            ImGui::InputText(text.c_str(), buf.data(), buf.size());
            buf.resize(strlen(buf.data()));
            return buf;
        },
        "InputFloat", [](ImGuiHandler&, const std::string& text, float value) -> float {
            ImGui::InputFloat(text.c_str(), &value);
            return value;
        },
        "InputInt", [](ImGuiHandler&, const std::string& text, int value) -> int {
            ImGui::InputInt(text.c_str(), &value);
            return value;
        },
        "Separator", [](ImGuiHandler&) {
            ImGui::Separator();
        },
        "SameLine", [](ImGuiHandler&) {
            ImGui::SameLine();
        },
        "Spacing", [](ImGuiHandler&) {
            ImGui::Spacing();
        },
        "SetWindowOpen", [](ImGuiHandler&, const std::string& title, bool isopen) {
            ImGuiHandler::set_windowopen(title, isopen);
        },
        "IsWindowOpen", [](ImGuiHandler&, const std::string& title) -> bool {
            return ImGuiHandler::is_windowopen(title);
        }
    );

    lua_state.set_function("RegisterEvent", [&](std::string name, sol::protected_function callback) {
        EventManager::addlistener(name, callback);
    });

    lua_state.set_function("HookFunction", [&](std::string name, sol::protected_function callback) {
        EventManager::addhook(name, callback);
    });

    lua_state.set_function("RegisterWindow", [&](const std::string& title, sol::protected_function draw, int togglekey, sol::object lockinput) {
        if (!globalscope) {
            std::cout << "[ImGui Lua] error: RegisterWindow is only available in the global scope!" << std::endl;
            return;
        }
        bool lock = true;
        if (lockinput.is<bool>()) {
            lock = lockinput.as<bool>();
        } else if (lockinput.is<int>()) {
            lock = (lockinput.as<int>() != 0);
        }
        ImGuiHandler::add_window(title, [draw]() {
            auto result = draw();
            if (!result.valid()) {
                sol::error err = result;
                std::cout << "[ImGui Lua] error: " << err.what() << std::endl;
            }
        }, togglekey, lock);
    });

    lua_state["player"] = Player();
    lua_state["system"] = System();
    lua_state["mission"] = Mission();
    lua_state["station"] = Station();
    lua_state["asset"] = Asset();
    lua_state["item"] = Item();
    lua_state["level"] = Level();
    lua_state["memoryutils"] = MemoryUtils();
    lua_state["imgui"] = ImGuiHandler();
    lua_state["engine"] = Engine();
}

void LuaManager::handle_coroutine(sol::thread script, sol::coroutine cor, const sol::protected_function_result& result, std::function<void()> on_complete, std::vector<sol::object> args)
{
    if (!result.valid()) {
        sol::error err = result;
        std::cout << "[LuaManager] runtime error: " << err.what() << std::endl;
        if (on_complete)
            on_complete();
        return;
    }
    // wait()
    if (result.get_type() == sol::type::number) {
        float wait_time = result.get<float>();
        tasks.emplace_back(LuaTask{std::move(script), std::move(cor), wait_time, on_complete, std::move(args)});
    } else {
        if (on_complete)
            on_complete();
    }
}

void LuaManager::execute_script(const std::string& filepath)
{
    try {
        std::filesystem::path path(filepath);
        std::string scriptdir = path.parent_path().string();
        std::string cpath = lua_state["package"]["path"];
        lua_state["package"]["path"] = cpath + ";" + scriptdir + "/?.lua";

        sol::load_result loaded_script = lua_state.load_file(filepath);
        if (!loaded_script.valid()) {
            sol::error err = loaded_script;
            std::cout << "[LuaManager] Load error: " << err.what() << std::endl;
            return;
        }
        globalscope = true;
        sol::protected_function_result result = loaded_script();
        globalscope = false;
        if (!result.valid()) {
            sol::error err = result;
            std::cout << "[LuaManager] runtime error: " << err.what() << std::endl;
        }
    } catch (const sol::error& err) {
        std::cout << "[LuaManager] Lua exception: " << err.what() << std::endl;
    }
}

void LuaManager::update(float dt)
{
    for (auto t = tasks.begin(); t != tasks.end();) {
        t->time_left -= dt;
        if (t->time_left <= 0.0f) {
            sol::thread script = std::move(t->script_thread);
            sol::coroutine cor = std::move(t->cor);
            auto on_complete = std::move(t->on_complete);
            auto args = std::move(t->args);
            t = tasks.erase(t);
            auto result = cor(sol::as_args(args));
            handle_coroutine(std::move(script), std::move(cor), result, on_complete, std::move(args));
        } else {
            ++t;
        }
    }
}

lua_State* LuaManager::getluastate()
{
    return lua_state.lua_state();
}