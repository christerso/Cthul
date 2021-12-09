#pragma once

#include "common.h"
#include <glm/vec2.hpp>
#include "timer.h"

namespace king
{
    constexpr double kPixelSpeed = 175000.0 / 84500.0;
    enum class Behaviour
    {
        kStatic,
        kArmy,
        kCharacter
    };

    enum class Action
    {
        GUARD = 0,
        MOVE,
    };

    class Entity
    {
    public:
        Entity() = default;
        Entity(const Entity& me) = default;
        void set_position(const glm::vec2& position) { m_position = position; }
        glm::vec2& get_position() { return m_position; }
        void set_scale(double scale) { m_scale = scale; }
        double get_scale() const { return m_scale; }
    protected:
        float m_velocity;
        Behaviour m_bevaviour = Behaviour::kStatic;
        glm::vec2 m_position;
        double m_scale;
    };

} // namespace king