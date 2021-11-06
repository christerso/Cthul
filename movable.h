#pragma once

#include "position.h"

#include <SDL_render.h>

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

    virtual void move(Origin origin) = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;

    void set_position(Position& pos) { position_ = pos; };
    Position& get_position() { return position_; }

protected:
    Behaviour behaviour_ = Behaviour::STATIC;
    Position position_;
};

} // namespace king