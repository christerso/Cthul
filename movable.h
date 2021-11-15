#pragma once

#include "position.h"

#include <SDL_render.h>
#include <SDL_rect.h>

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

    void set_position(Position& pos)
    {
        position_ = pos;
    };
    const Position& get_position() const { return position_; }

protected:
    Behaviour behaviour_ = Behaviour::STATIC;
    Position position_;
};

} // namespace king