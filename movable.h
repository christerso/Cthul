#pragma once

#include "common.h"
#include <glm/vec2.hpp>
#include "timer.h"

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
        virtual void set_velocity(float velocity) = 0;
        float get_velocity_() const { return velocity_; }
        void set_position(glm::vec2& pos)
        {
            position_ = pos;
        };
        glm::vec2& get_position() { return position_; }

    protected:
        float velocity_;
        Behaviour behaviour_ = Behaviour::STATIC;
        glm::vec2 position_;
    };

} // namespace king