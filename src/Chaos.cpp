#include "Chaos.hpp"

#include <chrono>
#include <cstring>

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Module.hpp"
#include "Modules/Tier1.hpp"

#include "Command.hpp"
#include "Game.hpp"

#include "Utils/SDK.hpp"

Chaos chaos;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Chaos, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, chaos);

Chaos::Chaos()
{
    this->modules = new Modules();
    this->cheats = new Cheats();
    this->plugin = new Plugin();
    this->game = Game::CreateNew();
}

bool Chaos::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
    console = new Console();
    if (!console->Init())
        return false;

    if (this->game) {
        this->game->LoadOffsets();

        this->modules->AddModule<Tier1>(&tier1);
        this->modules->InitAll();

        if (tier1->hasLoaded) {
            this->cheats->Init();

            this->modules->AddModule<Engine>(&engine);
            this->modules->InitAll();

            if (engine->hasLoaded) {
                this->StartPluginThread();
                this->StartMainThread();

                console->PrintActive("Loaded chaos-plugin, Version %s\n", this->Version());
                return true;
            } else {
                console->Warning("chaos: Failed to load engine module!\n");
            }
        } else {
            console->Warning("chaos: Failed to load tier1 module!\n");
        }
    } else {
        console->Warning("chaos: Game not supported!\n");
    }

    console->Warning("chaos: Plugin failed to load!\n");
    return false;
}
void Chaos::Unload()
{
    this->Cleanup();
}
const char* Chaos::GetPluginDescription()
{
    return CHAOS_SIGNATURE;
}

void Chaos::Cleanup()
{
    if (console)
        console->Print("chaos: Cya!\n");

    this->Stop();

    this->mainIsRunning = false;
    this->mainThread.join();

    if (this->cheats)
        this->cheats->Shutdown();

    if (this->modules)
        this->modules->ShutdownAll();

    SAFE_UNLOAD(this->cheats);
    SAFE_UNLOAD(this->game);
    SAFE_UNLOAD(this->plugin);
    SAFE_UNLOAD(this->modules);
    SAFE_UNLOAD(console);
}

void Chaos::Start()
{
    this->isRunning = true;
}
void Chaos::Stop()
{
    this->isRunning = false;
    if (this->curCallback) {
        this->curCallback->Reset();
    }
}
void Chaos::Reset()
{
    if (this->curCallback) {
        this->curCallback->Reset();
    }

    this->callbacks = std::vector<RandomCallback*>(RandomCallback::list);
    this->curCallback = nullptr;

    for (auto& callback : this->callbacks) {
        if (!callback->Init()) {
            console->Warning("chaos: Failed to initialize callback: %s\n", callback->name);
        }
    }
}

bool Chaos::GetPlugin()
{
    static Interface* s_ServerPlugin = Interface::Create(MODULE("engine"), "ISERVERPLUGINHELPERS0", false);
    if (s_ServerPlugin) {
        auto m_Size = *reinterpret_cast<int*>((uintptr_t)s_ServerPlugin->ThisPtr() + CServerPlugin_m_Size);
        if (m_Size > 0) {
            auto m_Plugins = *reinterpret_cast<uintptr_t*>((uintptr_t)s_ServerPlugin->ThisPtr() + CServerPlugin_m_Plugins);
            for (int i = 0; i < m_Size; i++) {
                auto ptr = *reinterpret_cast<CPlugin**>(m_Plugins + sizeof(uintptr_t) * i);
                if (!std::strcmp(ptr->m_szName, CHAOS_SIGNATURE)) {
                    this->plugin->ptr = ptr;
                    this->plugin->index = i;
                    return true;
                }
            }
        }
    }
    return false;
}

// We don't use any callbacks
void Chaos::StartPluginThread()
{
    this->pluginThread = std::thread([this]() {
        GO_THE_FUCK_TO_SLEEP(1000);
        if (!this->GetPlugin()) {
            console->DevWarning("chaos: Failed to find itself in the plugin list!\n");
        } else {
            this->plugin->ptr->m_bDisable = true;
        }
    });
    this->pluginThread.detach();
}
// Main loop
void Chaos::StartMainThread()
{
    this->mainIsRunning = true;
    this->mainThread = std::thread([this]() {
        this->Reset();
        while (this->mainIsRunning) {
            while (this->isRunning) {
                // Standard delay
                auto delay = chaos_delay.GetInt();
                if (delay == 0) {
                    auto max = chaos_delay_upper_bound.GetInt();
                    auto min = chaos_delay_lower_bound.GetInt();
                    delay = std::rand() % (max - min + 1) + min;
                }

                auto then = std::chrono::steady_clock::now() + std::chrono::seconds(std::max(delay, 10));
                while (this->isRunning) {
                    if (std::chrono::steady_clock::now() < then) {
                        GO_THE_FUCK_TO_SLEEP(1);
                        continue;
                    }

                    // Another delay if we hit a loading screen or menu
                    then = std::chrono::steady_clock::now();
                    while (this->isRunning) {
                        auto notInGame = !engine->hoststate->m_activeGame
                            || engine->hoststate->m_currentState != HOSTSTATES::HS_RUN;

                        then = (notInGame)
                            ? std::chrono::steady_clock::now() + std::chrono::seconds(5)
                            : then;

                        if (std::chrono::steady_clock::now() < then) {
                            GO_THE_FUCK_TO_SLEEP(1);
                            continue;
                        }

                        // Dispatch a reset on previous callback
                        if (this->curCallback) {
                            this->curCallback->Reset();
                        }
                        this->curCallback = nullptr;

                        // Handle queue logic for given mode
                        auto mode = chaos_mode.GetInt();
                        if (this->callbacks.size() == 0) {
                            if (mode == 1) {
                                this->isRunning = false;
                                this->Reset();
                                console->Print("Chaos ended!\n");
                                break;
                            } else if (mode >= 2) {
                                this->Reset();
                            }
                        }

                        // Dispatch random callback
                        auto index = std::rand() % this->callbacks.size();
                        this->curCallback = this->callbacks.at(index);
                        this->curCallback->Dispatch();
                        console->Print("%s\n", this->curCallback->name);

                        // Handle mode again
                        if (mode > 0) {
                            this->callbacks.erase(this->callbacks.begin() + index);
                        }

                        break;
                    }

                    break;
                }
            }
            GO_THE_FUCK_TO_SLEEP(1);
        }
    });
}
// Might fix potential deadlock
#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_DETACH) {
        chaos.Cleanup();
    }
    return TRUE;
}
#endif

#pragma region Unused callbacks
void Chaos::Pause()
{
}
void Chaos::UnPause()
{
}
void Chaos::LevelInit(char const* pMapName)
{
}
void Chaos::ServerActivate(void* pEdictList, int edictCount, int clientMax)
{
}
void Chaos::GameFrame(bool simulating)
{
}
void Chaos::LevelShutdown()
{
}
void Chaos::ClientFullyConnect(void* pEdict)
{
}
void Chaos::ClientActive(void* pEntity)
{
}
void Chaos::ClientDisconnect(void* pEntity)
{
}
void Chaos::ClientPutInServer(void* pEntity, char const* playername)
{
}
void Chaos::SetCommandClient(int index)
{
}
void Chaos::ClientSettingsChanged(void* pEdict)
{
}
int Chaos::ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen)
{
    return 0;
}
int Chaos::ClientCommand(void* pEntity, const void*& args)
{
    return 0;
}
int Chaos::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID)
{
    return 0;
}
void Chaos::OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue)
{
}
void Chaos::OnEdictAllocated(void* edict)
{
}
void Chaos::OnEdictFreed(const void* edict)
{
}
#pragma endregion
