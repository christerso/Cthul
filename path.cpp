#include "path.h"

using namespace king;

void Path::add_curve(const Curve& curve, const int samples)
{
    std::lock_guard lock{ path_mtx_ };
    curves_.push_back(curve);
    samples_.push_back(samples);
}

double Path::update_path_samples()
{
    std::lock_guard lock{ path_mtx_ };
    double distance = 0.f;
    calculated_path_.clear();
    for (int i = 0; i < curves_.size(); i++)
    {
        for (float t = 0.f; t < 1.0f; t += 1.0f / samples_[i])  // NOLINT(cert-flp30-c)
        {
            auto res = curves_[i].calculate_vector_points(t);
            calculated_path_.emplace_back(res);
        }

        for (auto& entry : curves_)
        {
            distance += glm::distance(entry.p0, entry.p3);
        }
    }
    return distance;
}
bool Path::check_path()
{
    std::lock_guard lock{ path_mtx_ };
    return !curves_.empty();
}

std::vector<glm::vec2> Path::get_path()
{
    std::lock_guard lock{ path_mtx_ };
    return calculated_path_;
}

void Path::clear_path()
{
    std::lock_guard lock{ path_mtx_ };
    calculated_path_.clear();
    curves_.clear();
    samples_.clear();
}