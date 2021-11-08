#pragma once

namespace king
{
enum class Level
{
    kGround = 0,
    kUnderground,
    kAir,
    kWater
};

struct Position
{
    float pos_x{};
    float pos_y{};
};

} // namespace king