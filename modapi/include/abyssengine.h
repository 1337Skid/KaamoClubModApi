#ifndef ABYSSENGINE_H
#define ABYSSENGINE_H

#include <Game/structs.h>

class AbyssEngine {
    public:
        static AEString newstring(const wchar_t* str)
        {
            AEString r{};
            r.size = (uint32_t)(wcslen(str) + 1);
            r.text = (wchar_t*)VirtualAlloc(nullptr, sizeof(wchar_t) * r.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            wcscpy_s(r.text, r.size, str);
            return r;
        }

        template<typename T>
        static AEArray<T>* newarray(uint32_t n)
        {
            auto* a = (AEArray<T>*)VirtualAlloc(nullptr, sizeof(AEArray<T>), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            a->size = a->size2 = n;
            a->data = (T*)VirtualAlloc(nullptr, sizeof(T) * n, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            memset(a->data, 0, sizeof(T) * n);
            return a;
        }
};

#endif