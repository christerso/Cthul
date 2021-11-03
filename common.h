#pragma once

#include <random>
#include <cmath>

namespace common
{
static const int seed_val = 19680821;
static std::mt19937 m_generator;
const double pi = 3.14159265358979323846;

inline void clear_bit(int& target, const int bit)
{
    target &= ~(1 << bit);
}

inline void set_bit(int& target, const int bit)
{
    target |= 1 << bit;
}

inline void toggle_bit(int& target, const int bit)
{
    target ^= 1 << bit;
}

inline bool check_bit(int& target, const int bit)
{
    return target & (1 << bit);
}

inline int get_random_value_within_range(const int start, const int end)
{
    const std::uniform_int_distribution<int> uniform_dist(start, end);
    return uniform_dist(m_generator);
}

inline float get_random_value_within_range(const float start, const float end)
{
    const std::uniform_real_distribution<float> uniform_dist(start, end);
    return uniform_dist(m_generator);
}

inline double get_random_value_within_range(const double start, const double end)
{
    const std::uniform_real_distribution<double> uniform_dist(start, end);
    return uniform_dist(m_generator);
}

template <typename T>
T normalize(const T value, const T min, const T max)
{
    return (value - min) / (max - min);
}

inline float lerp(float value, float start, float end)
{
    return start + (end - start) * value;
}

// The distance can be defined as a straight line between 2 points.
inline int get_euclidean_distance(const int x1, const int y1, const int x2, const int y2)
{
    return static_cast<int>(std::sqrt((std::abs(x1 - x2) * 2) + (std::abs(y1 - y2) * 2)));
}

// All 8 adjacent cells from the given point can be reached by one unit
inline int get_chebychev_distance(const int x1, const int y1, const int x2, const int y2)
{
    return std::max(std::abs(x1 - x2), std::abs(y1 - y2));
}

// The distance between two points is the sum of the (absolute) differences of their coordinates.
// E.g. it only costs 1 unit for a straight move, but 2 if one wants to take a crossed move.
inline int get_manhattan_distance(const int x1, const int y1, const int x2, const int y2)
{
    return std::abs(std::abs((x1 - x2)) + std::abs((y1 - y2)));
}

// Bezier curve
// --------------------------------------------------------------------------------------------------------
inline double bezier_curve(const float t, const float p0, const float p1, const float p2, const float p3)
{
    const auto t2 = t * t;
    const auto t3 = t2 * t;

    return p0 + 3 * (p1 - p0) * t + 3 * (p2 - 2 * p1 + p0) * t2 + (p3 - 3 * (p2 - p1) - p0) * t3;
}

// Catmull Rom
// ---------------------------------------------------------------------------------------------------------

inline double catmull_rom_spline(const float t, const float p0, const float p1, const float p2, const float p3)
{
    const auto c1 = 1.0 * p1;
    const auto c2 = -0.5 * p0 + 0.5 * p2;
    const auto c3 = 1.0 * p0 + -2.5 * p1 + 2.0 * p2 + -0.5 * p3;
    const auto c4 = -0.5 * p0 + 1.5 * p1 + -1.5 * p2 + 0.5 * p3;

    return ((c4 * t + c3) * t + c2) * t + c1;
}

inline bool is_in_rectangle(const double center_x, const double center_y, const double radius, const double x,
                            const double y)
{
    return x >= center_x - radius && x <= center_x + radius && y >= center_y - radius && y <= center_y + radius;
}

inline bool is_point_in_circle(const double center_x, const double center_y, const double radius, const double x,
                               const double y)
{
    if (is_in_rectangle(center_x, center_y, radius, x, y))
    {
        auto dx = center_x - x;
        auto dy = center_y - y;

        dx *= dx;
        dy *= dy;
        const auto distance_squared = dx + dy;
        const auto radius_squared = radius * radius;

        return distance_squared <= radius_squared;
    }

    return false;
}

// Used to plot a sight-line from point x0, y0 to x1, y1 using Bresenheim's line algorithm.
inline void get_sightline(int x0, int y0, const int x1, const int y1)
{
    const auto dx = std::abs(x1 - x0);
    const auto sx = x0 < x1 ? 1 : -1;
    const auto dy = -std::abs(y1 - y0);
    const auto sy = y0 < y1 ? 1 : -1;
    auto err = dx + dy; /* error value e_xy */
    for (;;)
    {
        // setPixel(x0, y0);

        if ((x0 == x1) && (y0 == y1))
            break;

        const auto e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

}