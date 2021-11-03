#pragma once

namespace king
{
enum class Level
{
    GROUND = 0,
    UNDERGROUND,
    AIR,
    WATER
};

class Position
{
public:
    float pos_x{};
    float pos_y{};
};

} // namespace king