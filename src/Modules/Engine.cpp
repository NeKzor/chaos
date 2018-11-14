#include "Engine.hpp"

#include "Console.hpp"

#include "Interface.hpp"
#include "Offsets.hpp"
#include "Utils.hpp"
#include "Variable.hpp"

Variable sv_cheats;

Engine::Engine()
    : Module()
{
}
bool Engine::Init()
{
    if (auto engine = Interface::Create(MODULE("engine"), "VEngineClient0", false)) {
        this->GetMaxClients = engine->Original<_GetMaxClients>(Offsets::GetMaxClients);
        this->GetActiveSplitScreenPlayerSlot = engine->Original<_GetActiveSplitScreenPlayerSlot>(Offsets::GetActiveSplitScreenPlayerSlot);

        typedef void* (*_GetClientState)();
        auto ClientCmd = engine->Original(Offsets::ClientCmd);
        auto GetClientState = Memory::Read<_GetClientState>(ClientCmd + Offsets::GetClientStateFunction);
        auto cl = Interface::Create(GetClientState(), false);
        if (cl) {
            auto SetSignonState = cl->Original(Offsets::SetSignonState);
            auto HostState_OnClientConnected = Memory::Read(SetSignonState + Offsets::HostState_OnClientConnected);
            this->hoststate = Memory::Deref<CHostState*>(HostState_OnClientConnected + Offsets::hoststate);
        }

        Memory::Read<_Cbuf_AddText>(ClientCmd + Offsets::Cbuf_AddText, &this->Cbuf_AddText);
    }

    sv_cheats = Variable("sv_cheats");

    return this->hasLoaded = this->hoststate
        && this->GetMaxClients
        && this->GetActiveSplitScreenPlayerSlot
        && this->Cbuf_AddText
        && !!sv_cheats;
}
void Engine::Shutdown()
{
}
void Engine::SendToCommandBuffer(const char* text, int delay)
{
#ifdef _WIN32
    auto slot = this->GetActiveSplitScreenPlayerSlot();
#else
    auto slot = this->GetActiveSplitScreenPlayerSlot(nullptr);
#endif
    this->Cbuf_AddText(slot, text, delay);
}

Engine* engine;
