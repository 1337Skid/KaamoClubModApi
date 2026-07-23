#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H
#include <Game/structs.h>
#include <string>
#include <functional>
#include <vector>

class TouchButton {
    private:
        SingleTouchButton *_ptr = nullptr;
        struct CustomButton {
            std::function<void()> onclick;
            int state;
            SingleTouchButton *ptr;
        };
    public:
        static inline std::vector<CustomButton> created_buttons;
        TouchButton(const std::string &text, const std::string &subtext, int x, int y, int textcolor, int state, sol::main_protected_function onclick);
        ~TouchButton();
        std::string tostring(void);
        SingleTouchButton* getptr() const { return _ptr; }
        void settext(const std::string &text);
        void show(void);
        void hide(void);
        static void draw(SingleTouchButton *btn);
        static void refreshbtnsprites(int* btnContainer);
};
#endif