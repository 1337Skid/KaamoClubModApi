#include "memoryutils.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>
#include <Game/level.h>
#include <Game/kiplayer.h>
#include <Game/touchbutton.h>
#include <thread>
#include <chrono>
#include "patches.h"
#include "offset.h"
#include "abyssengine.h"

TouchButton::TouchButton(const std::string &text, const std::string &subtext, int x, int y, int textcolor, int state, int ismenutouchwindow, sol::main_protected_function onclick)
{
    SingleTouchButton *btn = reinterpret_cast<SingleTouchButton*>(AbyssEngine::memory_allocate(sizeof(SingleTouchButton)));
    int text_len = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    wchar_t *text_buf = reinterpret_cast<wchar_t*>(AbyssEngine::memory_allocate(text_len * sizeof(wchar_t)));
    
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, text_buf, text_len);
    int subtext_len = MultiByteToWideChar(CP_UTF8, 0, subtext.c_str(), -1, nullptr, 0);
    wchar_t *subtext_buf = reinterpret_cast<wchar_t*>(AbyssEngine::memory_allocate(subtext_len * sizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, subtext.c_str(), -1, subtext_buf, subtext_len);
    CustomButton cbtn;
    cbtn.onclick = onclick;
    cbtn.state = state;
    std::memset(btn, 0, sizeof(SingleTouchButton));
    btn->m_nId = 1234;
    btn->m_sText = text_buf;
    btn->m_nTextSize = static_cast<uint32_t>(text_len - 1);
    btn->m_sTextNumber = subtext_buf;
    btn->m_nTextNumberEnable = static_cast<uint32_t>(subtext_len - 1);
    btn->m_nTextColor = textcolor;
    btn->m_nX = x;
    btn->m_nY = y;
    btn->field_6C = x;
    btn->field_70 = y;
    btn->m_pFont = 0;
    btn->field_10 = 0;
    btn->m_sTextRight = nullptr;
    btn->m_nTextMode = -1;
    btn->m_nIconSide = -1;
    btn->m_nSpritePressed = 67;
    btn->m_nSpriteMidPressed = 37;
    btn->m_nSpriteRightPressed = 38;
    btn->m_nSpriteNormal = 66;
    btn->m_nSpriteMidNormal = 34;
    btn->m_nSpriteRightNormal = 35;
    btn->m_nSpriteHighlighted = 67;
    btn->m_nSpriteMidHighlighted = 37;
    btn->m_nSpriteRightHighlighted = 38;
    btn->m_nSpriteBg = 0;
    btn->m_nSpriteOverlay = 0;
    btn->m_nIcon = 0;
    btn->field_58 = 200;
    btn->m_nBtnType = 0;
    btn->field_60 = 0;
    btn->m_nHeight = 34;
    btn->field_78 = 30;
    btn->m_nWidth = 200;
    btn->m_nLeftWidth = 22;
    btn->field_84 = 100;
    btn->m_nRightWidth = 22;
    btn->m_nMidWidth = 156;
    btn->m_nTextOffsetX = 40;
    btn->m_nTextOffsetY = 9;
    btn->m_bIsPressed = false;
    btn->m_bIsHighlighted = false;
    btn->m_bIsVisible = true;
    btn->m_bIsDisabled = false;
    btn->m_nTextColor2 = 0x70005f;
    btn->m_fProgress = 0.0f;
    btn->field_A8 = 0;
    btn->field_AC = -2;
    _ptr = btn;
    cbtn.ptr = _ptr;
    created_buttons.push_back(std::move(cbtn));
}
TouchButton::~TouchButton() {};

std::string TouchButton::tostring(void)
{
    std::ostringstream oss;
    oss << "TouchButton(0x" << reinterpret_cast<void*>(_ptr) << ")";
    return oss.str();
}

void TouchButton::draw(SingleTouchButton *btn)
{
    int a1 = reinterpret_cast<int>(btn);
    uintptr_t func = Offset::TOUCHBUTTON_DRAW;

    __asm {
        mov edi, a1
        call func
    }
}

void TouchButton::settext(const std::string &text)
{
    int text_len = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    wchar_t *text_buf = reinterpret_cast<wchar_t*>(AbyssEngine::memory_allocate(text_len * sizeof(wchar_t)));

    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, text_buf, text_len);
    AbyssEngine::memory_free(_ptr->m_sText);
    _ptr->m_sText = text_buf;
    _ptr->m_nTextSize = static_cast<uint32_t>(text_len - 1);
}