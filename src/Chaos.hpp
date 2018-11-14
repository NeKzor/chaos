#pragma once
#include <atomic>
#include <thread>
#include <vector>

#include "Modules/Module.hpp"

#include "Command.hpp"
#include "Game.hpp"
#include "Cheats.hpp"
#include "Plugin.hpp"
#include "RandomCallback.hpp"
#include "Utils/SDK.hpp"

#define CHAOS_VERSION "1.0"
#define CHAOS_BUILD __TIME__ " " __DATE__
#define CHAOS_WEB "https://nekzor.github.io/chaos"

#define SAFE_UNLOAD(ptr) \
    if (ptr) {           \
        delete ptr;      \
        ptr = nullptr;   \
    }

enum class ChaosMode {
    Default,
    Once,
    OnceAndReset
};

class Chaos : public IServerPluginCallbacks {
public:
    Game* game;
    Plugin* plugin;
    Modules* modules;
    Cheats* cheats;

    RandomCallback* curCallback;
    std::vector<RandomCallback*> callbacks;

    ChaosMode mode;
    std::atomic<bool> hasStarted;

private:
    std::thread pluginThread;
    std::thread mainThread;
    std::atomic<bool> isRunning;

public:
    Chaos();

    virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
    virtual void Unload();
    virtual void Pause();
    virtual void UnPause();
    virtual const char* GetPluginDescription();
    virtual void LevelInit(char const* pMapName);
    virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax);
    virtual void GameFrame(bool simulating);
    virtual void LevelShutdown();
    virtual void ClientFullyConnect(void* pEdict);
    virtual void ClientActive(void* pEntity);
    virtual void ClientDisconnect(void* pEntity);
    virtual void ClientPutInServer(void* pEntity, char const* playername);
    virtual void SetCommandClient(int index);
    virtual void ClientSettingsChanged(void* pEdict);
    virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen);
    virtual int ClientCommand(void* pEntity, const void*& args);
    virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID);
    virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue);
    virtual void OnEdictAllocated(void* edict);
    virtual void OnEdictFreed(const void* edict);

    const char* Version() { return CHAOS_VERSION; }
    const char* Build() { return CHAOS_BUILD; }
    const char* Website() { return CHAOS_WEB; }

    bool GetPlugin();
    void Cleanup();

    void ResetCallbacks();

private:
    void StartPluginThread();
    void StartMainThread();
};

extern Chaos chaos;
