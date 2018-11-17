#pragma once
#include "Command.hpp"
#include "Variable.hpp"

class Cheats {
public:
    void Init();
    void Shutdown();
};

extern Variable chaos_mode;
extern Variable chaos_time;
extern Variable chaos_time_lower_bound;
extern Variable chaos_time_upper_bound;
extern Variable chaos_cooldown;
extern Variable chaos_cooldown_lower_bound;
extern Variable chaos_cooldown_upper_bound;

extern Command chaos_start;
extern Command chaos_stop;
extern Command chaos_reset_queue;
extern Command chaos_set_seed;
extern Command chaos_set_quantity;
extern Command chaos_states;
