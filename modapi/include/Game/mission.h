#ifndef MISSION_H
#define MISSION_H

class Mission {
    private:
        static inline uintptr_t global_status = 0;
    public:
        static void init(void);
        static int getid(void);
        static void setid(int value);
        static int getcompletedsidemissions(void);
        static void setcompletedsidemission(int value);
};
#endif