#pragma once

namespace king
{
    enum class Origin
    {
        kPlayer,
        kEntity,
    };

    class Movable
    {
    public:
        virtual void move(Origin origin) = 0;
        virtual void set_velocity(double velocity) { m_velocity = velocity; }
        double get_velocity_() const { return m_velocity; }
    private:
        double m_velocity;
        double m_stamina;
    };
}
