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
    int x{};
    int y{};
};

} // namespace king