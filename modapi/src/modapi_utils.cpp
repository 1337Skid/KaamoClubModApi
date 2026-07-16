#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <tlhelp32.h>
#include <vector>
#include <codecvt>
#include <sol/sol.hpp>
#include <map>
#include <string>
#include "modapi_utils.h"
#include "luamanager.h"
#include "memoryutils.h"
#include <iomanip>
#include "eventmanager.h"
#include "abyssengine.h"
#include <Game/player.h>
#include <Game/system.h>
#include <Game/station.h>
#include <Game/mission.h>
#include <Game/asset.h>

std::string ModApiUtils::gettimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tmp{};
    localtime_s(&tmp, &time);
    std::ostringstream oss;
    oss << std::put_time(&tmp, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

std::string ModApiUtils::getexception_name(DWORD code)
{
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION: return "ACCESS_VIOLATION";
        case EXCEPTION_STACK_OVERFLOW: return "STACK_OVERFLOW";
        case EXCEPTION_ILLEGAL_INSTRUCTION: return "ILLEGAL_INSTRUCTION";
        case EXCEPTION_INT_DIVIDE_BY_ZERO: return "DIVIDE_BY_ZERO";
        case EXCEPTION_BREAKPOINT: return "BREAKPOINT";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "FLT_DIVIDE_BY_ZERO";
        case EXCEPTION_INT_OVERFLOW: return "INT_OVERFLOW";
        default: return "what thee helllll";
    }
}

LONG WINAPI ModApiUtils::crashhandler(EXCEPTION_POINTERS* ep)
{
    static volatile LONG guard = 0;

    if (InterlockedExchange(&guard, 1) != 0)
        return EXCEPTION_EXECUTE_HANDLER;
    std::string ts = gettimestamp();
    std::ostringstream log;
    log << "========================================\n";
    log << "  KaamoClubModAPI - crash\n";
    log << "  Version: 1.0.4\n";
    log << "========================================\n\n";
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    log << "[Exception]\n";
    log << "  Code: " << getexception_name(code) << "\n";
    log << "  Address: 0x" << std::hex << reinterpret_cast<uintptr_t>(ep->ExceptionRecord->ExceptionAddress) << std::dec << "\n";
    if (code == EXCEPTION_ACCESS_VIOLATION && ep->ExceptionRecord->NumberParameters >= 2) {
        log << "  Type: " << (ep->ExceptionRecord->ExceptionInformation[0] == 1 ? "WRITE" : "READ") << "\n";
        log << "  Target: 0x" << std::hex << ep->ExceptionRecord->ExceptionInformation[1] << std::dec << "\n";
    }
    CONTEXT* ctx = ep->ContextRecord;
    log << "\n[Registers]\n" << std::hex;
    log << "  EAX=" << ctx->Eax << "  EBX=" << ctx->Ebx << "  ECX=" << ctx->Ecx << "  EDX=" << ctx->Edx << "\n";
    log << "  ESI=" << ctx->Esi << "  EDI=" << ctx->Edi << "  ESP=" << ctx->Esp << "  EBP=" << ctx->Ebp << "\n";
    log << "  EIP=" << ctx->Eip << "  EFlags=" << ctx->EFlags << "\n" << std::dec;
    log << "\n[Module at Crash Address]\n";
    HMODULE handlemodule = nullptr;
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCTSTR>(ep->ExceptionRecord->ExceptionAddress), &handlemodule)) {
        char modulepath[MAX_PATH];
        GetModuleFileNameA(handlemodule, modulepath, MAX_PATH);
        uintptr_t base = reinterpret_cast<uintptr_t>(handlemodule);
        uintptr_t offset = reinterpret_cast<uintptr_t>(ep->ExceptionRecord->ExceptionAddress) - base;
        log << "  Name: " << modulepath << "\n";
        log << "  Base: 0x" << std::hex << base << "\n";
        log << "  Offset: 0x" << offset << std::dec << "\n";
        FreeLibrary(handlemodule);
    } else {
        log << "  which module crashed???\n";
    }
    log << "\n[Stack Trace]\n";
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
    CONTEXT ctxCopy = *ctx;
    STACKFRAME64 sf = {};
    sf.AddrPC.Offset = ctx->Eip;
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrFrame.Offset = ctx->Ebp;
    sf.AddrFrame.Mode = AddrModeFlat;
    sf.AddrStack.Offset = ctx->Esp;
    sf.AddrStack.Mode = AddrModeFlat;
    for (int i = 0; i < 32; i++) {
        if (!StackWalk64(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(), &sf, &ctxCopy, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
            break;
        if (sf.AddrPC.Offset == 0)
            break;
        log << "  [" << i << "] 0x" << std::hex << sf.AddrPC.Offset;
        char symbuf[sizeof(SYMBOL_INFO) + 256] = {};
        SYMBOL_INFO* sym = reinterpret_cast<SYMBOL_INFO*>(symbuf);
        sym->SizeOfStruct = sizeof(SYMBOL_INFO);
        sym->MaxNameLen = 255;
        DWORD64 disp64 = 0;
        if (SymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &disp64, sym))
            log << "  " << sym->Name << " +0x" << disp64;
        IMAGEHLP_LINE64 line = {};
        line.SizeOfStruct = sizeof(line);
        DWORD linedisplay = 0;
        if (SymGetLineFromAddr64(GetCurrentProcess(), sf.AddrPC.Offset, &linedisplay, &line))
            log << "  (" << line.FileName << ":" << std::dec << line.LineNumber << ")";
        log << "\n";
    }
    SymCleanup(GetCurrentProcess());
    if (!std::filesystem::exists("crashlogs"))
        std::filesystem::create_directory("crashlogs");
    std::string logpath = "crashlogs/crash_" + ts + ".txt";
    std::ofstream file(logpath);
    if (file.is_open()) {
        file << log.str();
        file.flush();
        file.close();
    }
    MessageBoxW(NULL, L"Uh oh, looks like the game crashed.\n\n" L"Crash logs have been saved in the folder \"crashlogs\".", L"Game crash", MB_OK | MB_ICONERROR);
    return EXCEPTION_EXECUTE_HANDLER;
}

void ModApiUtils::load_mods(LuaManager *luamanager)
{    
    if (!std::filesystem::exists("mods") || !std::filesystem::is_directory("mods"))
        std::filesystem::create_directory("mods");
    for (const auto& entry : std::filesystem::directory_iterator("mods")) {
        if (entry.is_directory()) {
            std::string mod_path = entry.path().string();
            std::string init_lua = mod_path + "/init.lua";
            if (std::filesystem::exists(init_lua)) {
                std::cout << "[+] Loaded mod: " << entry.path().filename().string() << std::endl;
                luamanager->execute_script(init_lua);
            }
        }
    }    
}

// credits https://gist.github.com/danzek/d6a0e4a48a5439e7f808ed1497f6268e
std::string ModApiUtils::w2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}

std::wstring ModApiUtils::s2w(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(str);
}