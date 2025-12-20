#ifndef ASSET_H
#define ASSET_H

class Asset {
    private:
        static inline uintptr_t globals_canvas = 0;
    public:
        static void init(void);
        static std::string getassetfilepath(unsigned int offset);
        static void setassetfilepath(unsigned int offset, const std::string value);
};
#endif