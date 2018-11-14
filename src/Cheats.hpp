#pragma once
#include "Command.hpp"
#include "Variable.hpp"

class Cheats {
public:
    void Init();
    void Shutdown();
};

extern Variable chaos_mode;
extern Variable chaos_delay;
extern Variable chaos_delay_lower_bound;
extern Variable chaos_delay_upper_bound;

extern Command chaos_start;
extern Command chaos_stop;
extern Command chaos_reset;
