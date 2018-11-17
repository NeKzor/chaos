#include "Chaos.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <stdarg.h>

#include "Modules/Client.hpp"
#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Module.hpp"
#include "Modules/Tier1.hpp"

#include "Command.hpp"
#include "Game.hpp"
#include "Utils.hpp"

Chaos chaos;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Chaos, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, chaos);

Chaos::Chaos()
{
    this->modules = new Modules();
    this->cheats = new Cheats();
    this->plugin = new Plugin();
    this->game = Game::CreateNew();
    this->curState = nullptr;
    this->queue = std::vector<State*>();
    this->mode = ChaosMode::Default;
    this->SetSeed(static_cast<unsigned int>(time(0)));
    this->cooldown = true;
    this->clients = std::vector<void*>();
}

// Used callbacks
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
            this->modules->AddModule<Client>(&client);
            this->modules->InitAll();

            if (engine && client && engine->hasLoaded && client->hasLoaded) {
                this->StartMainThread();

                console->PrintActive("Loaded chaos-plugin, Version %s\n", this->Version());
                return true;
            } else {
                console->Warning("chaos: Failed to load engine and client modules!\n");
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
void Chaos::LevelShutdown()
{
    console->DevMsg("Chaos::LevelShutdown\n");
    this->clients.clear();
}
void Chaos::ClientFullyConnect(void* pEdict)
{
    console->DevMsg("Chaos::ClientFullyConnect -> pEdict: %p\n", pEdict);
    this->clients.push_back(pEdict);
}

// Utilities
void Chaos::BufferCommand(const char* text, int delay)
{
#ifdef _WIN32
    auto slot = engine->GetActiveSplitScreenPlayerSlot();
#else
    auto slot = engine->GetActiveSplitScreenPlayerSlot(nullptr);
#endif
    engine->Cbuf_AddText(slot, text, delay);
}
void Chaos::EachClient(const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    char data[1024];
    vsnprintf(data, sizeof(data), fmt, argptr);
    va_end(argptr);

    for (const auto& client : this->clients) {
        engine->ClientCommand(nullptr, client, data);
    }
}
void Chaos::Chat(const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    char data[1024];
    vsnprintf(data, sizeof(data), fmt, argptr);
    va_end(argptr);

    client->ChatPrintf(client->g_HudChat->ThisPtr(), 0, 0, "%c%s", TextColor::COLOR_LOCATION, data);
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

// Controls
void Chaos::Start()
{
    this->isRunning = true;
    this->cooldown = true;
}
void Chaos::Stop()
{
    this->isRunning = false;
    if (this->curState) {
        this->curState->Reset();
    }
}
void Chaos::Reset()
{
    if (this->curState) {
        this->curState->Reset();
    }
    this->curState = nullptr;

    this->queue = std::vector<State*>();
    for (const auto& state : State::list) {
        for (int i = 0; i < state->quantity; ++i) {
            this->queue.push_back(state);
        }
    }

    for (auto& state : this->queue) {
        if (!state->Init()) {
            console->Warning("chaos: Failed to initialize state: %s\n", state->name);
        }
    }
}
void Chaos::SetSeed(int seed)
{
    std::srand(this->seed = seed);
}
const int Chaos::GetDelay()
{
    if (this->cooldown) {
        auto delay = chaos_cooldown.GetInt();
        if (delay < 0) {
            auto max = chaos_cooldown_upper_bound.GetInt();
            auto min = chaos_cooldown_lower_bound.GetInt();
            delay = std::rand() % (max - min + 1) + min;
        }
        return std::max(delay, 0);
    }

    auto delay = chaos_time.GetInt();
    if (delay < 0) {
        auto max = chaos_time_upper_bound.GetInt();
        auto min = chaos_time_lower_bound.GetInt();
        delay = std::rand() % (max - min + 1) + min;
    }

    // Anything below 10 seconds doesn't make sense, right?
    return std::max(delay, 10);
}
void Chaos::Run()
{
    if (!this->cooldown) {
        // Dispatch a reset on previous state
        if (this->curState) {
            this->curState->Reset();
        }
        this->curState = nullptr;
        return;
    }

    // Handle queue logic for given mode
    auto mode = chaos_mode.GetInt();
    if (this->queue.size() == 0) {
        if (mode == 1) {
            this->isRunning = false;
            this->Reset();
            console->Print("Chaos ended!\n");
            return;
        } else if (mode >= 2) {
            this->Reset();
        }
    }

    // Dispatch random state
    auto index = std::rand() % this->queue.size();
    this->curState = this->queue.at(index);
    this->curState->Dispatch();
    console->DevMsg("chaos: %s\n", this->curState->name);

    if (chaos_spoiler.GetBool()) {
        chaos.Chat("%s", this->curState->name);
    }

    // Handle mode again
    if (mode > 0) {
        this->queue.erase(this->queue.begin() + index);
    }
}

// Main loop
void Chaos::StartMainThread()
{
    this->mainIsRunning = true;
    this->mainThread = std::thread([this]() {
        this->Reset();
        while (this->mainIsRunning) {
            while (this->isRunning) {
                auto then = std::chrono::steady_clock::now() + std::chrono::seconds(this->GetDelay());
                while (this->isRunning) {
                    if (std::chrono::steady_clock::now() < then) {
                        GO_THE_FUCK_TO_SLEEP(1);
                        continue;
                    }

                    // Delay again if we hit a loading screen or menu
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

                        this->Run();
                        this->cooldown = !this->cooldown;
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
