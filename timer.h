#pragma once

#include <chrono>

namespace king
{
using tick = std::chrono::steady_clock::time_point;
class Timer
{
public:
    static Timer* instance();
    static void release();
    void reset();
    float delta_time();
    void time_scale(float t);
    double time_scale();
    void update();
private:
    Timer();
    ~Timer();
    static Timer* s_instance_;
    tick start_tick_ {};
    std::chrono::duration<float> elapsed_tick_ {};
    float time_scale_;

};
}
