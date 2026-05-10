#ifndef KIPLAYER_H
#define KIPLAYER_H
#include <Game/structs.h>

class KIPlayer {
    private:
        int* _ptr = nullptr;
    public:
        KIPlayer(int* ptr);
        ~KIPlayer();

        std::string tostring(void);
        void setroute(int* routeptr);
};
#endif