#pragma once
#include "Utils.hpp"

#define CHAOS_SIGNATURE \
    new char[20] { 102, 111, 114, 32, 109, 121, 32, 108, 111, 118, 101, 32, 98, 97, 105, 115, 116, 101, 114, 00 }

// CServerPlugin
#define CServerPlugin_m_Size 16
#define CServerPlugin_m_Plugins 4

class Plugin {
public:
    CPlugin* ptr;
    int index;

public:
    Plugin();
};
