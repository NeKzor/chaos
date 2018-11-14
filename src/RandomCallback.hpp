#pragma once
#include <stdint.h>
#include <vector>

class RandomCallback;

using _RandomCallback = void (*)(RandomCallback* state, bool lucky);

class RandomCallback {
public:
    bool initialized;
    const char* name;
    _RandomCallback callback;

    static std::vector<RandomCallback*> list;

public:
    RandomCallback(const char* name, _RandomCallback callback);

    bool Init();
    void Dispatch();
    void Reset();

    static void ResetAll();
};

#define CHAOS(name)                                                \
    void name##_callback(RandomCallback* state, bool lucky);       \
    RandomCallback name##_state = RandomCallback(#name, name##_callback); \
    void name##_callback(RandomCallback* state, bool lucky)
