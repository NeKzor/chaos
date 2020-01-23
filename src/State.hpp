#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class State;
using _InitCallback = void (*)(State* state);

enum class CommandType {
    NotSpecified,
    ServerSide,
    ClientSide
};

class State {
public:
    const char* name;
    _InitCallback initCallback;
    int quantity;
    std::string on;
    std::string off;
    bool isTimed;
    bool onceOnly;
    bool turnOffBeforeLoading;
    CommandType type;

public:
    State(const char* name, _InitCallback initCallback);
    void Init();

    static std::vector<State*>& GetList();
};

#define CHAOS(name)                                  \
    void name##_init_callback(State* state);         \
    State name = State(#name, name##_init_callback); \
    void name##_init_callback(State* state)
