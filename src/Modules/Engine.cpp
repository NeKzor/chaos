#include "Engine.hpp"

#include "Interface.hpp"
#include "Offsets.hpp"
#include "Utils.hpp"
#include "Variable.hpp"

Variable sv_cheats;
Variable sv_gravity;
Variable sv_friction;
Variable sv_maxvelocity;

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
        Memory::Deref<void*>((uintptr_t)this->Cbuf_AddText + Offsets::s_CommandBuffer, &this->s_CommandBuffer);

        if (this->s_CommandBuffer) {
            auto m_bWaitEnabled = reinterpret_cast<bool*>((uintptr_t)s_CommandBuffer + Offsets::m_bWaitEnabled);
            auto m_bWaitEnabled2 = reinterpret_cast<bool*>((uintptr_t)m_bWaitEnabled + Offsets::CCommandBufferSize);
            *m_bWaitEnabled = *m_bWaitEnabled2 = true;
        }
    }

    if (auto g_VEngineServer = Interface::Create(this->Name(), "VEngineServer0", false)) {
        this->ClientCommand = g_VEngineServer->Original<_ClientCommand>(Offsets::ClientCommand);
    }

    sv_cheats = Variable("sv_cheats");
    sv_gravity = Variable("sv_gravity");
    sv_friction = Variable("sv_friction");
    sv_maxvelocity = Variable("sv_maxvelocity");

    if (!!sv_cheats && !!sv_gravity && !!sv_friction && !!sv_maxvelocity) {
        sv_cheats.Modify(FCVAR_NOTIFY);
        sv_gravity.Modify(FCVAR_NOTIFY);
        sv_friction.Modify(FCVAR_NOTIFY | FCVAR_DEVELOPMENTONLY | FCVAR_HIDDEN, FCVAR_CHEAT);
        sv_maxvelocity.Modify(FCVAR_NOTIFY | FCVAR_DEVELOPMENTONLY | FCVAR_HIDDEN, FCVAR_CHEAT);
    }

    return this->hasLoaded = this->hoststate
        && this->GetActiveSplitScreenPlayerSlot
        && this->Cbuf_AddText
        && this->s_CommandBuffer
        && this->ClientCommand
        && !!sv_cheats
        && !!sv_gravity
        && !!sv_friction
        && !!sv_maxvelocity;
}
void Engine::Shutdown()
{
    if (this->s_CommandBuffer) {
        auto m_bWaitEnabled = reinterpret_cast<bool*>((uintptr_t)s_CommandBuffer + Offsets::m_bWaitEnabled);
        auto m_bWaitEnabled2 = reinterpret_cast<bool*>((uintptr_t)m_bWaitEnabled + Offsets::CCommandBufferSize);
        *m_bWaitEnabled = *m_bWaitEnabled2 = false;
    }
}

Engine* engine;
