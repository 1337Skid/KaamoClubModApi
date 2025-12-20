#ifndef HOOKS_H
#define HOOKS_H

class Hooks {
    private:
        static constexpr uintptr_t GLOBALS_INIT_ADDR = 0x0044B20C;
        static constexpr uintptr_t GLOBALS_GALAXY = 0x0060AF3C;

        using globals_init = uintptr_t(__stdcall*)(uintptr_t, uintptr_t, uintptr_t);
        static globals_init oldglobals_init;
        static uintptr_t __stdcall globals_init_hook(uintptr_t a, uintptr_t b, uintptr_t c);

        static void injectsystems(void);
    public:
        static void init(void);
};

#endif