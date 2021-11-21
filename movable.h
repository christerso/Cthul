#pragma once

#include "common.h"

namespace king
{

enum class Behaviour
{
    STATIC,
    ARMY,
    CHARACTER
};

enum class Origin
{
    PLAYER,
    AI
};

enum class Action
{
    GUARD = 0,
    MOVE,
};

class MovableEntity
{
public:
    MovableEntity() = default;
    MovableEntity(const MovableEntity& me) = default;
    virtual void move(Origin origin) = 0;

    void set_position(common::Position& pos)
    {
        position_ = pos;
    };
    common::Position& get_position() { return position_; }

protected:
    Behaviour behaviour_ = Behaviour::STATIC;
    common::Position position_;
};

} // namespace king