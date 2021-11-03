#pragma once
#pragma once

#include "army.h"
#include "movable.h"
#include <functional>
#include <thread_pool.hpp>
// Anything that wants to be added to be moved by the threadpool
// inherits the Movement class. Each class having inherited the movemnt class
// needs to set what type of behaviour the movement should be

namespace king
{

enum class MovementType
{
    ARMY,
    CHARACTER
};

using ThreadMethod = std::function<void()>;

class ThreadManager
{
public:
    bool quit = false;
    thread_pool pool;
};

} // namespace king