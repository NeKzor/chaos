#pragma once
#include <stdint.h>
#include <vector>

class State;
using _StateCallback = void (*)(State* state, bool lucky);

enum class StateAction {
    None,
    Enable,
    EnableAndShutdown
};

class State {
public:
    bool isInitialized;
    const char* name;
    _StateCallback callback;
    int quantity;
    StateAction action;

    static std::vector<State*> list;

public:
    State(const char* name, _StateCallback callback, StateAction action = StateAction::None);

    bool Init();
    void Dispatch();
    void Reset();

    static void ResetAll();
};

#define CHAOS(name)                                 \
    void name##_callback(State* state, bool lucky); \
    State name = State(#name, name##_callback);     \
    void name##_callback(State* state, bool lucky)
#define CHAOS2(name)                                                 \
    void name##_callback(State* state, bool lucky);                  \
    State name = State(#name, name##_callback, StateAction::Enable); \
    void name##_callback(State* state, bool lucky)
#define CHAOS3(name)                                                            \
    void name##_callback(State* state, bool lucky);                             \
    State name = State(#name, name##_callback, StateAction::EnableAndShutdown); \
    void name##_callback(State* state, bool lucky)
