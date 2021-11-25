#pragma once

#include "astar.h"
#include <vector>
#include <random>
#include <cmath>
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/gtx/spline.hpp>

namespace common
{
    static constexpr int kSeedVal = 19680821;
    static std::mt19937 g_generator;
    constexpr double kPi = 3.14159265358979323846;

    // Bezier curve
// --------------------------------------------------------------------------------------------------------
    // Hermite

    template <typename T>
    T cubic_hermite(T p0, T p1, T p2, T p3, const float t)
    {
        glm::vec2 a = -p0 / 2.0f + (3.0f * p1) / 2.0f - (3.0f * p2) / 2.0f + p3 / 2.0f;
        glm::vec2 b = p0 - (5.0f * p1) / 2.0f + 2.0f * p2 - p3 / 2.0f;
        glm::vec2 c = -p0 / 2.0f + p2 / 2.0f;
        glm::vec2 d = p1;

        return a * t * t * t + b * t * t + c * t + d;
    }

    // ---------------------------------------------------------------------------------------------------------

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

    struct Curve
    {
        glm::vec2 p0;
        glm::vec2 p1;
        glm::vec2 p2;
        glm::vec2 p3;

        [[nodiscard]] glm::vec2 calculate_vector_points(const float t)
        {
            return cubic_hermite<glm::vec2>(p0, p1, p2, p3, t);
        }
    };

    class Path
    {
    public:
        void add_curve(const Curve& curve, const int samples)
        {
            curves_.push_back(curve);
            samples_.push_back(samples);
        }

        void update_path_samples()
        {
            calculated_path_.clear();

            for (int i = 0; i < curves_.size(); i++)
            {
                for (float t = 0.f; t < 1.0f; t += 1.0f / samples_[i])  // NOLINT(cert-flp30-c)
                {
                    calculated_path_.emplace_back(curves_[i].calculate_vector_points(t));
                }
            }
        }

        std::vector<glm::vec2>& get_path()
        {
            return calculated_path_;
        }

        bool check_path() const { return !curves_.empty(); }

        void clear_path()
        {
            calculated_path_.clear();
            curves_.clear();
            samples_.clear();
        }

    private:
        std::vector<glm::vec2> calculated_path_;
        std::vector<Curve> curves_;
        std::vector<int> samples_;
    };

    static glm::vec2 get_square(const int tile_width, const int tile_height, const glm::vec2& pos)
    {
        const auto tile_square = glm::vec2{ pos.x / tile_width, pos.y / tile_height };
        return tile_square;
    }

    static glm::vec2 get_square_center(const int tile_width, const int tile_height, const glm::vec2& pos)
    {
        return glm::vec2{ pos.x * tile_width + (tile_width / 2), pos.y * tile_height + (tile_height / 2) };
    }

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

    template <typename T>
    struct RangeRand
    {
        inline static T get_random_value_within_range(const T start, const T end);
    };

    template <>
    inline int RangeRand<int>::get_random_value_within_range(const int start, const int end)
    {
        const std::uniform_int_distribution<> uniform_dist(start, end);
        return uniform_dist(g_generator);
    }

    template <>
    inline float RangeRand<float>::get_random_value_within_range(const float start, const float end)
    {
        const std::uniform_real_distribution<> uniform_dist(start, end);
        return static_cast<float>(uniform_dist(g_generator));
    }

    template<>
    inline double RangeRand<double>::get_random_value_within_range(const double start, const double end)
    {
        const std::uniform_real_distribution<> uniform_dist(start, end);
        return uniform_dist(g_generator);
    }

    template <typename T>
    T normalize(const T value, const T min, const T max)
    {
        return (value - min) / (max - min);
    }

    template <typename T>
    float lerp(const T value, const T start, const T end)
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