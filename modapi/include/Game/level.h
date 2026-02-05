#ifndef LEVEL_H
#define LEVEL_H
#include <Game/structs.h>
class Level {
    private:
        static inline Globals_appManager** globals_appmanager = 0;
        struct CustomRadioMsg {
            int part_one;
            int part_two;
            int part_three;
            int part_four;
            int x;
            int y;
            int scale_x;
            int scale_y;
            std::wstring name;
            std::wstring content;
        };
    public:
        static inline std::vector<CustomRadioMsg> created_radiomessages;
        static void init(void);
        static void createradiomessage(const std::string& name, const std::string& content, sol::table imageinfo);
};
#endif