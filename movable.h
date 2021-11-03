#pragma once

#include "position.h"

namespace king
{

enum class Behaviour
{
    STATIC,
    ARMY,
    CHARACTER
};

enum class Action
{
    GUARD = 0,
    MOVE,
};

class Movable
{
public:
    virtual void move(Position pos) = 0;

    void setup();
protected:
    Behaviour behaviour_ = Behaviour::STATIC;
    Position position;
};

} // namespace king