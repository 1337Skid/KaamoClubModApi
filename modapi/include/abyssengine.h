#ifndef ABYSSENGINE_H
#define ABYSSENGINE_H

#include <Game/structs.h>
#include <cstdlib>

class AbyssEngine {
    public:
        typedef uintptr_t(__cdecl* malloc_t)(size_t size);
        static uintptr_t memory_allocate(size_t size)
        {
            static uintptr_t ptr = 0x0052D150;
            malloc_t pmalloc = *reinterpret_cast<malloc_t*>(ptr);
            uintptr_t result = 0;

            if (!pmalloc) {
                return 0;
            }

            // SEH
            __try {
                result = pmalloc(size);
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                std::cout << "wtf" << std::endl;
                result = 0;
            }
            return result;
        }
        typedef void(__cdecl* free_t)(void* ptr);
        static void memory_free(void *ptr)
        {
            static uintptr_t fptr = 0x0052D14C;
            free_t pfree = *reinterpret_cast<free_t*>(fptr);

            if (!pfree) {
                std::cout << "nahhhhh" << std::endl;
                return;
            }

            // SEH
            __try {
                pfree(ptr);
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                std::cout << "wtf" << std::endl;
            }
        }
        static AEString newstring(const wchar_t* str)
        {
            AEString r = { 0 };

            if (!str)
                return r;
            r.size = static_cast<uint32_t>((wcslen(str) + 1));
            r.text = reinterpret_cast<wchar_t*>(memory_allocate(sizeof(wchar_t) * r.size));
            if (r.text != nullptr) {
                wcscpy_s(r.text, r.size, str);
            } else {
                r.size = 0;
            }
            return r;
        }
        static AEString newstring(const char* str)
        {
            AEString r = { 0 };

            if (!str)
                return r;
            int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
            if (len <= 0)
                return r;
            r.size = static_cast<uint32_t>(len);
            r.text = reinterpret_cast<wchar_t*>(memory_allocate(sizeof(wchar_t) * r.size));
            if (!r.text) {
                r.size = 0;
                return r;
            }
            MultiByteToWideChar(CP_UTF8, 0, str, -1, r.text, len);
            return r;
        }
        template<typename T>
        static AEArray<T>* newarray(uint32_t n)
        {
            auto* a = (AEArray<T>*)memory_allocate(sizeof(AEArray<T>));
            
            if (!a)
                return nullptr;
            a->size = n;
            a->size2 = n;
            a->data = (T*)memory_allocate(sizeof(T) * n);
            if (a->data)
                memset(a->data, 0, sizeof(T) * n);
            return a;
        }
};

#endif