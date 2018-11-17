#pragma once
#include "Module.hpp"
#include "Utils.hpp"
#include "Variable.hpp"

extern Variable sv_cheats;

class Engine : public Module {
public:
    CHostState* hoststate = nullptr;

    using _Cbuf_AddText = void(__cdecl*)(int slot, const char* pText, int nTickDelay);
#ifdef _WIN32
    using _GetActiveSplitScreenPlayerSlot = int (*)();
#else
    using _GetActiveSplitScreenPlayerSlot = int (*)(void* thisptr);
#endif
     using _ClientCommand = int(__func*)(void* thisptr, void* pEdict, const char* szFmt, ...);

    _GetActiveSplitScreenPlayerSlot GetActiveSplitScreenPlayerSlot = nullptr;
    _Cbuf_AddText Cbuf_AddText = nullptr;
    _ClientCommand ClientCommand = nullptr;

    Engine();
    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("engine"); }
};

extern Engine* engine;
