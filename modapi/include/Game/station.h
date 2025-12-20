#ifndef STATION_H
#define STATION_H

class Station {
    private:
        static inline uintptr_t globals_status = 0;
    public:
        static void init(void);
        static int getid(void);
        static void setid(int value);
        static std::string getname(void);
        static void setname(const std::string value);
        static int gettechlevel(void);
        static void settechlevel(int value);
        
        static bool isvoid(void);
};
#endif