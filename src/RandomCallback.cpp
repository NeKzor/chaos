#include "RandomCallback.hpp"

#include <vector>

RandomCallback::RandomCallback(const char* name, _RandomCallback callback)
    : initialized(false)
    , name(name)
    , callback(callback)
{
    RandomCallback::list.push_back(this);
}
bool RandomCallback::Init()
{
    this->initialized = false;
    this->callback(this, false);
    return this->initialized;
}
void RandomCallback::Dispatch()
{
    this->callback(this, true);
}
void RandomCallback::Reset()
{
    this->callback(this, false);
}

std::vector<RandomCallback*> RandomCallback::list;
