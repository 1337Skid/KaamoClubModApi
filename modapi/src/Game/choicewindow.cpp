#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/level.h>
#include <Game/kiplayer.h>
#include <Game/touchbutton.h>
#include <Game/choicewindow.h>
#include <thread>
#include <chrono>
#include "patches.h"
#include "offset.h"
#include "abyssengine.h"

ChoiceWindow::ChoiceWindow(const std::string &title, const std::string &description, bool show_buttons, sol::main_protected_function onclick_yes, sol::main_protected_function onclick_no)
{
    SingleChoiceWindow *choicewindow = reinterpret_cast<SingleChoiceWindow*>(AbyssEngine::memory_allocate(sizeof(SingleChoiceWindow)));
    uintptr_t choicewindowctor_address = Offset::CHOICEWINDOW_CHOICEWINDOW;
    
    std::memset(choicewindow, 0, sizeof(SingleChoiceWindow));
    __asm {
        mov ecx, choicewindow
        call choicewindowctor_address
    }
    int tlen = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0);
    std::wstring wstitle(tlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, wstitle.data(), tlen);
    int slen = MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, nullptr, 0);
    std::wstring wsdescription(slen, 0);
    MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, wsdescription.data(), slen);
    CustomChoiceWindow customchoicewindow;
    customchoicewindow.ptr = choicewindow;
    customchoicewindow.onclick_yes = onclick_yes;
    customchoicewindow.onclick_no = onclick_no;
    customchoicewindow.initialized = false;
    customchoicewindow.title = std::move(wstitle);
    customchoicewindow.description = std::move(wsdescription);
    customchoicewindow.show_buttons = show_buttons;
    created_choicewindow.push_back(std::move(customchoicewindow));
    _ptr = choicewindow;
}

ChoiceWindow::~ChoiceWindow() {}

std::string ChoiceWindow::tostring(void)
{
    std::ostringstream oss;
    oss << "ChoiceWindow(0x" << reinterpret_cast<void*>(_ptr) << ")";
    return oss.str();
}

// TODO: it does the popup sound only one time... bcz of the set being called one time
void ChoiceWindow::show()
{
    CustomChoiceWindow *entry = nullptr;
    HangarWindow *hangarwindow = nullptr;
    MGame *mgame = nullptr;
    MenuTouchWindow *menutouchwindow = nullptr;

    for (auto& c : ChoiceWindow::created_choicewindow) {
        if (c.ptr == _ptr) {
            entry = &c;
            break;
        }
    }
    if (!entry)
        return;
    if (ChoiceWindow::last_active_window == ChoiceWindow::HANGAR_WINDOW) {
        hangarwindow = reinterpret_cast<HangarWindow*>(ChoiceWindow::active_hangarwindow);
        if (!hangarwindow || !MemoryUtils::IsValidPointer(hangarwindow))
            return;
    } else if (ChoiceWindow::last_active_window == ChoiceWindow::MGAME_WINDOW) {
        mgame = reinterpret_cast<MGame*>(ChoiceWindow::active_mgamewindow);
        if (!mgame || !MemoryUtils::IsValidPointer(mgame))
            return;
    } else if (ChoiceWindow::last_active_window == ChoiceWindow::MENU_TOUCH_WINDOW) {
        menutouchwindow = reinterpret_cast<MenuTouchWindow*>(ChoiceWindow::active_menutouchwindow);
        if (!menutouchwindow || !MemoryUtils::IsValidPointer(menutouchwindow))
            return;
    }
    if (!entry->initialized) {
        AEString title_str = AbyssEngine::newstring(entry->title.c_str());
        AEString description_str = AbyssEngine::newstring(entry->description.c_str());
        // TODO: put localized strings
        AEString yes_str = AbyssEngine::newstring(L"Yes");
        AEString no_str = AbyssEngine::newstring(L"No");
        AEString ok_str = AbyssEngine::newstring(L"OK");
        // no __asm here because it's not a custom namecall!!
        typedef void* (__stdcall* choicewindowset_t)(SingleChoiceWindow*, AEString*, AEString*, char, AEString*, AEString*, AEString*);
        choicewindowset_t choicewindowset = reinterpret_cast<choicewindowset_t>(Offset::CHOICEWINDOW_SET);
        choicewindowset(_ptr, &title_str, &description_str, entry->show_buttons ? 1 : 0, &yes_str, &no_str, &ok_str);
        entry->initialized = true;
    }
    SingleTouchButton *ref = nullptr;
    bool hangarsprites = (ChoiceWindow::last_active_window == ChoiceWindow::HANGAR_WINDOW) || (ChoiceWindow::last_active_window == ChoiceWindow::MGAME_WINDOW);
    SingleChoiceWindow *orig_window = nullptr;
    if (hangarwindow)
        orig_window = hangarwindow->m_pChoiceWindow2;
    else if (mgame)
        orig_window = mgame->m_pChoiceWindow;
    else if (menutouchwindow)
        orig_window = menutouchwindow->m_pChoiceWindow;
    if (!hangarsprites && menutouchwindow) {
        AEArray<SingleTouchButton*>* m_pButtons = menutouchwindow->m_pButtons;
        if (MemoryUtils::IsValidPointer(m_pButtons)) {
            if (m_pButtons->size > 0 && MemoryUtils::IsValidPointer(m_pButtons->data))
                ref = m_pButtons->data[0];
        }
    } else if (mgame) {
        if (orig_window && MemoryUtils::IsValidPointer(orig_window)) {
            if (orig_window->m_pBtnYes && MemoryUtils::IsValidPointer(orig_window->m_pBtnYes))
                ref = orig_window->m_pBtnYes;
            else if (orig_window->m_pBtnNo && MemoryUtils::IsValidPointer(orig_window->m_pBtnNo))
                ref = orig_window->m_pBtnNo;
        }
        if (!ref && mgame->m_pMenuTouchWindow && MemoryUtils::IsValidPointer(mgame->m_pMenuTouchWindow)) {
            AEArray<SingleTouchButton*>* m_pButtons = mgame->m_pMenuTouchWindow->m_pButtons;
            if (MemoryUtils::IsValidPointer(m_pButtons)) {
                if (m_pButtons->size > 0 && MemoryUtils::IsValidPointer(m_pButtons->data)) {
                    for (uint32_t i = 0; i < m_pButtons->size; ++i) {
                        if (m_pButtons->data[i] && MemoryUtils::IsValidPointer(m_pButtons->data[i])) {
                            ref = m_pButtons->data[i];
                            break;
                        }
                    }
                }
            }
        }
        if (!ref && ChoiceWindow::active_menutouchwindow) {
            MenuTouchWindow* active_menu = reinterpret_cast<MenuTouchWindow*>(ChoiceWindow::active_menutouchwindow);
            if (MemoryUtils::IsValidPointer(active_menu)) {
                AEArray<SingleTouchButton*>* m_pButtons = active_menu->m_pButtons;
                if (MemoryUtils::IsValidPointer(m_pButtons)) {
                    if (m_pButtons->size > 0 && MemoryUtils::IsValidPointer(m_pButtons->data) && m_pButtons->data[0] && MemoryUtils::IsValidPointer(m_pButtons->data[0]))
                        ref = m_pButtons->data[0];
                }
            }
        }
    }
    // TODO: sometimes broken on MGame....
    auto hackyfix = [](SingleTouchButton* btn, SingleTouchButton* ref, int windowtype) {
        if (!btn)
            return;
        if (windowtype == ChoiceWindow::HANGAR_WINDOW) {
            btn->m_nSpriteNormal = 64;
            btn->m_nSpritePressed = 65;
            btn->m_nSpriteHighlighted = 67;
            btn->m_nSpriteMidNormal = 34;
            btn->m_nSpriteRightNormal = 35;
            btn->m_nSpriteMidPressed = 37;
            btn->m_nSpriteRightPressed = 38;
            btn->m_nSpriteMidHighlighted = 37;
            btn->m_nSpriteRightHighlighted = 38;
        } else if (windowtype == ChoiceWindow::MGAME_WINDOW) {
            if (ref) {
                btn->m_nSpriteNormal = ref->m_nSpriteNormal;
                btn->m_nSpritePressed = ref->m_nSpritePressed;
                btn->m_nSpriteHighlighted = ref->m_nSpriteHighlighted;
                btn->m_nSpriteMidNormal = ref->m_nSpriteMidNormal;
                btn->m_nSpriteRightNormal = ref->m_nSpriteRightNormal;
                btn->m_nSpriteMidPressed = ref->m_nSpriteMidPressed;
                btn->m_nSpriteRightPressed = ref->m_nSpriteRightPressed;
                btn->m_nSpriteMidHighlighted = ref->m_nSpriteMidHighlighted;
                btn->m_nSpriteRightHighlighted = ref->m_nSpriteRightHighlighted;
            } else {
                btn->m_nSpriteNormal = 171;
                btn->m_nSpritePressed = 172;
                btn->m_nSpriteHighlighted = 67;
                btn->m_nSpriteMidNormal = 34;
                btn->m_nSpriteRightNormal = 35;
                btn->m_nSpriteMidPressed = 37;
                btn->m_nSpriteRightPressed = 38;
                btn->m_nSpriteMidHighlighted = 37;
                btn->m_nSpriteRightHighlighted = 38;
            }
        } else if (ref) {
            btn->m_nSpriteNormal = ref->m_nSpriteNormal;
            btn->m_nSpriteMidNormal = ref->m_nSpriteMidNormal;
            btn->m_nSpriteRightNormal = ref->m_nSpriteRightNormal;
            btn->m_nSpritePressed = ref->m_nSpritePressed;
            btn->m_nSpriteMidPressed = ref->m_nSpriteMidPressed;
            btn->m_nSpriteRightPressed = ref->m_nSpriteRightPressed;
            btn->m_nSpriteHighlighted = ref->m_nSpriteHighlighted;
            btn->m_nSpriteMidHighlighted = ref->m_nSpriteMidHighlighted;
            btn->m_nSpriteRightHighlighted = ref->m_nSpriteRightHighlighted;
        } 
        btn->m_nSpriteBg = 0;
        btn->m_nSpriteOverlay = 0;
    };
    hackyfix(_ptr->m_pBtnYes, ref, ChoiceWindow::last_active_window);
    hackyfix(_ptr->m_pBtnNo, ref, ChoiceWindow::last_active_window);
    entry->previousrealchoicewindow_ptr = orig_window;
    if (hangarwindow) {
        entry->prev_active = hangarwindow->m_bChoiceWindowActive;
        hangarwindow->m_pChoiceWindow2 = _ptr;
        hangarwindow->m_bChoiceWindowActive = true;
    } else if (mgame) {
        entry->prev_active = mgame->m_bChoiceWindowActive;
        mgame->m_pChoiceWindow = _ptr;
        mgame->m_bChoiceWindowActive = true;
        mgame->m_bRenderHUD = true;
        unsigned char enable_byte = 1;
        uintptr_t togglepause_address = Offset::MGAME_TOGGLEPAUSE;
        __asm {
            push ecx
            push edx
            mov eax, mgame
            mov cl, enable_byte
            mov edx, togglepause_address
            call edx
            pop edx
            pop ecx
        }
        uintptr_t mgamepausesounds_address = Offset::MGAME_PAUSESOUNDS;
        __asm {
            push ecx
            push edx
            mov eax, mgame
            mov edx, mgamepausesounds_address
            call edx
            pop edx
            pop ecx
        }
    } else if (menutouchwindow) {
        entry->prev_active = menutouchwindow->m_bChoiceWindowActive;
        menutouchwindow->m_pChoiceWindow = _ptr;
        menutouchwindow->m_bChoiceWindowActive = true;
    }
    _ptr->m_bShowButtons = true;
}

void ChoiceWindow::hide()
{
    restore_state(_ptr);
}

void ChoiceWindow::restore_state(SingleChoiceWindow *customchoicewindow)
{
    if (ChoiceWindow::last_active_window == ChoiceWindow::HANGAR_WINDOW) {
        if (!ChoiceWindow::active_hangarwindow)
            return;
        HangarWindow *hangarwindow = reinterpret_cast<HangarWindow*>(ChoiceWindow::active_hangarwindow);
        if (hangarwindow->m_pChoiceWindow2 == customchoicewindow) {
            CustomChoiceWindow *entry = nullptr;
            for (auto &custom : created_choicewindow) {
                if (custom.ptr == customchoicewindow) {
                    entry = &custom;
                    break;
                }
            }
            if (entry) {
                hangarwindow->m_pChoiceWindow2 = entry->previousrealchoicewindow_ptr;
                hangarwindow->m_bChoiceWindowActive = entry->prev_active;
            } else {
                hangarwindow->m_pChoiceWindow2 = nullptr;
                hangarwindow->m_bChoiceWindowActive = false;
            }
        }
    } else if (ChoiceWindow::last_active_window == ChoiceWindow::MGAME_WINDOW) {
        if (!ChoiceWindow::active_mgamewindow)
            return;
        MGame *mgame = reinterpret_cast<MGame*>(ChoiceWindow::active_mgamewindow);
        if (mgame->m_pChoiceWindow == customchoicewindow) {
            CustomChoiceWindow* entry = nullptr;
            for (auto &custom : created_choicewindow) {
                if (custom.ptr == customchoicewindow) {
                    entry = &custom;
                    break;
                }
            }
            if (entry) {
                mgame->m_pChoiceWindow = entry->previousrealchoicewindow_ptr;
                mgame->m_bChoiceWindowActive = entry->prev_active;
            } else {
                mgame->m_pChoiceWindow = nullptr;
                mgame->m_bChoiceWindowActive = false;
            }
            mgame->m_bRenderHUD = true;
            unsigned char enable_byte = 0;
            uintptr_t togglepause_address = Offset::MGAME_TOGGLEPAUSE;
            __asm {
                push ecx
                push edx
                mov eax, mgame
                mov cl, enable_byte
                mov edx, togglepause_address
                call edx
                pop edx
                pop ecx
            }
            typedef void(__cdecl* MGame_ReleaseTouches_t)();
            auto release_touches = reinterpret_cast<MGame_ReleaseTouches_t>(0x49058C);
            release_touches();
        }
    } else if (ChoiceWindow::last_active_window == ChoiceWindow::MENU_TOUCH_WINDOW) {
        if (!ChoiceWindow::active_menutouchwindow)
            return;
        MenuTouchWindow *menutouchwindow = reinterpret_cast<MenuTouchWindow*>(ChoiceWindow::active_menutouchwindow);
        if (menutouchwindow->m_pChoiceWindow == customchoicewindow) {
            CustomChoiceWindow *entry = nullptr;
            for (auto &custom : created_choicewindow) {
                if (custom.ptr == customchoicewindow) {
                    entry = &custom;
                    break;
                }
            }
            if (entry) {
                menutouchwindow->m_pChoiceWindow = entry->previousrealchoicewindow_ptr;
                menutouchwindow->m_bChoiceWindowActive = entry->prev_active;
            } else {
                menutouchwindow->m_pChoiceWindow = nullptr;
                menutouchwindow->m_bChoiceWindowActive = false;
            }
            //menutouchwindow->field_16E = false;
            //menutouchwindow->field_16F = false;
        }
    }
    customchoicewindow->m_bShowButtons = false;
}