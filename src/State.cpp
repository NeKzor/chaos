#include "State.hpp"

#include <vector>

State::State(const char* name, _InitCallback initCallback)
    : name(name)
    , initCallback(initCallback)
    , quantity(1)
    , on()
    , off()
    , isTimed(false)
    , onceOnly(true)
    , turnOffBeforeLoading(false)
    , type(CommandType::NotSpecified)
{
    State::GetList().push_back(this);
}
void State::Init()
{
    this->initCallback(this);
}
std::vector<State*>& State::GetList()
{
    static std::vector<State*> list;
    return list;
}
