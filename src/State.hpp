#pragma once
#include <stdint.h>
#include <vector>

class State;

using _StateCallback = void (*)(State* state, bool lucky);

class State {
public:
    bool initialized;
    const char* name;
    _StateCallback callback;
    int quantity;

    static std::vector<State*> list;

public:
    State(const char* name, _StateCallback callback);

    bool Init();
    void Dispatch();
    void Reset();

    static void ResetAll();
};

#define CHAOS(name)                                     \
    void name##_callback(State* state, bool lucky);     \
    State name = State(#name, name##_callback); \
    void name##_callback(State* state, bool lucky)
