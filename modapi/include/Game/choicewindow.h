#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H
#include <Game/structs.h>
#include <string>
#include <functional>
#include <vector>

class ChoiceWindow {
    private:
        SingleChoiceWindow *_ptr = nullptr;
        struct CustomChoiceWindow {
            sol::main_protected_function onclick_yes;
            sol::main_protected_function onclick_no;
            SingleChoiceWindow *ptr;
            bool initialized = false;
            std::wstring title;
            std::wstring description;
            bool show_buttons = true;
            SingleChoiceWindow *previousrealchoicewindow_ptr = nullptr;
            bool prev_active = false;
        };
    public:
        enum ActiveWindowType {
            NONE,
            MENU_TOUCH_WINDOW,
            HANGAR_WINDOW,
            MGAME_WINDOW
        };
        static inline std::vector<CustomChoiceWindow> created_choicewindow;
        static inline uintptr_t active_menutouchwindow = 0;
        static inline uintptr_t active_hangarwindow = 0;
        static inline uintptr_t active_mgamewindow = 0;
        static inline ActiveWindowType last_active_window = NONE;
        ChoiceWindow(const std::string &title, const std::string &description, bool show_buttons, sol::main_protected_function onclick_yes, sol::main_protected_function onclick_no);
        ~ChoiceWindow();
        std::string tostring(void);
        void show(void);
        void hide(void);
        static void restore_state(SingleChoiceWindow* cw);
};
#endif