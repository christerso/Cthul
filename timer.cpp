#include "timer.h"
#include <chrono>

using namespace king;

Timer* Timer::s_instance_ = nullptr;

Timer* Timer::instance()
{
    if (s_instance_ == nullptr)
    {
        s_instance_ = new Timer();
    }
    return s_instance_;
}

void Timer::release()
{
    delete s_instance_;
    s_instance_ = nullptr;
}

void Timer::reset()
{
    start_tick_ = std::chrono::steady_clock::now();
    elapsed_tick_ = {};
}

float Timer::delta_time()
{
    return elapsed_tick_.count();
}

void Timer::time_scale(float t)
{
    time_scale_ = t;
}

double Timer::time_scale()
{
    return time_scale_;
}

void Timer::update()
{
    elapsed_tick_ = std::chrono::steady_clock::now() - start_tick_;
}

Timer::Timer()
    : time_scale_(1.0)
{
    reset();
}

Timer::~Timer()
{
}
