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
    if (auto engine = Interface::Create(this->Name(), "VEngineClient0", false)) {
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

    if (auto g_VEngineServer = Interface::Create(this->Name(), "VEngineServer0")) {
        this->ClientCommand = g_VEngineServer->Original<_ClientCommand>(Offsets::ClientCommand);
    }

    sv_cheats = Variable("sv_cheats");
    if (!!sv_cheats) {
        sv_cheats.Notify(false);
    }

    return this->hasLoaded = this->hoststate
        && this->GetActiveSplitScreenPlayerSlot
        && this->Cbuf_AddText
        && this->ClientCommand
        && !!sv_cheats;
}
void Engine::Shutdown()
{
}

Engine* engine;
