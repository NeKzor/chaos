#include "State.hpp"

#include <vector>

State::State(const char* name, _StateCallback callback)
    : isInitialized(false)
    , name(name)
    , callback(callback)
    , quantity(1)
{
    State::list.push_back(this);
}
bool State::Init()
{
    this->isInitialized = false;
    this->callback(this, false);
    return this->isInitialized;
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
