#pragma once

#include "common.h"

#include <vector>
#include <mutex>

#include <glm/vec2.hpp>
#include <glog/logging.h>

namespace king
{

    struct Curve
    {
        glm::vec2 p0;
        glm::vec2 p1;
        glm::vec2 p2;
        glm::vec2 p3;

        [[nodiscard]] glm::vec2 calculate_vector_points(const float t) const
        {
            return common::cubic_hermite<glm::vec2>(p0, p1, p2, p3, t);
        }
    };

    class Path
    {
    public:
        std::mutex path_mtx_;
        Path() {}
        Path(const Path & path)
            : calculated_path_(path.calculated_path_), curves_(path.curves_), samples_(path.samples_),
              distance_(path.distance_)
        {
            std::cout << "Copy Constructor called" << std::endl;
        }

        Path& operator=(const Path& path)
        {
            calculated_path_ = path.calculated_path_;
            curves_ = path.curves_;
            samples_ = path.samples_;
            distance_ = path.distance_;
        }

        void add_curve(const Curve& curve, const int samples);
        double update_path_samples();
        std::vector<glm::vec2> get_path();
        [[nodiscard]] bool check_path();
        void clear_path();

    private:
        std::vector<glm::vec2> calculated_path_;
        std::vector<Curve> curves_;
        std::vector<int> samples_;
        float distance_{};
    };
}
