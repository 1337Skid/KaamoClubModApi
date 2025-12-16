#ifndef MEMORYUTILS_H
#define MEMORYUTILS_H
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include "eventmanager.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

class MemoryUtils {
    public:
        static uintptr_t GetModuleBase(const char* modulename);
        static uintptr_t GetPointerAddress(uintptr_t baseaddr, const std::vector<unsigned int>& offsets);
        static std::wstring ReadRawString(uintptr_t address, size_t size = 256)
        {
            std::vector<wchar_t> buffer(size);

            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)address, buffer.data(), size * sizeof(wchar_t), NULL);
            buffer[size - 1] = L'\0';
            return std::wstring(buffer.data());
        }
        static std::string ReadWideString(uintptr_t addr, size_t size = 256)
        {
            std::wstring wstr = ReadRawString(addr, size);

            if (wstr.empty())
                return std::string();
            int readsize = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
            std::string strto(readsize, 0);
            WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strto[0], readsize, NULL, NULL);
            return strto;
        }
        static std::string ReadString(uintptr_t addr, size_t size = 256)
        {
            std::vector<char> buffer(size);
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)addr, buffer.data(), buffer.size(), nullptr);
            buffer[size - 1] = '\0';
            return std::string(buffer.data());
        }
        static void WriteWideString(uintptr_t addr, const std::string& str)
        {
            int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
            std::vector<wchar_t> buffer(size);
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), size);
            WriteProcessMemory(GetCurrentProcess(), (LPVOID)addr, buffer.data(), buffer.size() * sizeof(wchar_t), nullptr);
        }
        static void WriteString(uintptr_t addr, const std::string& str)
        {
            size_t size = str.size() + 1;

            WriteProcessMemory(GetCurrentProcess(), (LPVOID)addr, str.c_str(), size, nullptr);
        }

        template <typename T>
        static T Read(uintptr_t address)
        {
            T buffer;
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)address, &buffer, sizeof(T), NULL);
            return buffer;
        }

        template <typename T>
        static bool Write(uintptr_t address, T value)
        {
            return WriteProcessMemory(GetCurrentProcess(), (LPVOID)address, &value, sizeof(T), NULL);
        }
};
#endif