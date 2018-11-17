#include "Offsets.hpp"

namespace Offsets {

// CCvar
int RegisterConCommand;
int UnregisterConCommand;
int FindCommandBase;
int m_pConCommandList;

// CEngineClient
int ClientCmd;
int GetMaxClients;
int GetGameDirectory;
int GetActiveSplitScreenPlayerSlot;
int IsInCommentaryMode;

// ConVar
int InternalSetValue;
int InternalSetFloatValue;
int InternalSetIntValue;

// CClientState
int SetSignonState;
int Disconnect;

// CDemoPlayer
int IsPlayingBack;

// CEngineTool
int GetCurrentMap;

// CVEngineServer
int ClientCommand;

// Others
int GetClientStateFunction;
int cl;
int demoplayer;
int AutoCompletionFunc;
int HostState_OnClientConnected;
int hoststate;
int CL_IsRecordingMovie;
int m_szLevelName;
int Cbuf_AddText;
}
