#include "State.hpp"

#include <vector>

State::State(const char* name, _StateCallback callback)
    : initialized(false)
    , name(name)
    , callback(callback)
    , quantity(1)
{
    State::list.push_back(this);
}
bool State::Init()
{
    this->initialized = false;
    this->callback(this, false);
    return this->initialized;
}
void State::Dispatch()
{
    this->callback(this, true);
}
void State::Reset()
{
    this->callback(this, false);
}

std::vector<State*> State::list;
