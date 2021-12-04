#include "army.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>
#include <cmath>
#include <glm/gtx/norm.hpp>
#include "timer.h"
#include <chrono>
#include <ratio>

using namespace king;
constexpr double kNanoSecond = 1000000000;
Army::Army(Character& owner, glm::vec2& pos, Sprite* sprite, int army_size)
    : owner_(owner.get_id())
    , army_id_(to_string(boost::uuids::random_generator()())), sprite_(sprite),
    army_size_(army_size)
{
    set_position(pos);
    set_velocity(5.0f);
    LOG(INFO) << "created army: " << army_id_;
}

Army::~Army() {}

// a call to move will only happen if path_active = true
void Army::move(Origin origin)
{
    // get current time difference from last time this function was called

    const auto now = std::chrono::steady_clock::now();
    delta_counter += static_cast<int>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - time_point).count());
    time_point = now;

    if (delta_counter < kNanoSecond)
    {
        return;
    }
    const auto path = movement_path.get_path();
    if (path.empty())
    {
        return;
    }
    delta_counter = 0;
    current_path_position_ += common::kMovementPerSecondPer1Kmh * speed_;
    path_step_position_ = static_cast<size_t>(current_path_position_ / (path_distance / static_cast<double>(path.size())));
    std::cout << "Path Step:" << path_step_position_ << std::endl;
    position_ = path[path_step_position_];

    // take the path length in km and divide it by speed
    const auto path_update = path_distance - current_path_position_;
    time_path_will_take_ = path_update / (common::kMovementPerHour1kmh * speed_);
    // difference between the current path start point and the current position
    auto diff = glm::distance2(position_, path[path_position_]);

    // get position to measure how far we've got

    if (path_position_ >= path.size())
    {
        path_active = false;
        movement_path.clear_path();
        return;
    }

    // check if destination reached
    const auto distance = glm::distance2(position_, path[path_position_]);
    if (distance < 1.0f && path_position_ <= path.size())
    {
        ++path_position_;
    }
}

void Army::set_velocity(float velocity)
{
    velocity_ = velocity;
}

float Army::get_velocity() const
{
    return velocity_;
}

void Army::draw(SDL_Renderer* renderer)
{
    auto& pos = get_position();
    SDL_Rect origin{ static_cast<int>(pos.x), static_cast<int>(pos.y), sprite_->source_rect.w, sprite_->source_rect.h };
    scale_object(center_, origin, pos_, current_scale_);
    //SDL_RenderDrawRect(renderer, &pos_);
    SDL_RenderCopyEx(renderer, sprite_->texture, &sprite_->source_rect, &pos_, 0, &center_, SDL_FLIP_NONE);
}

const ArmyID& Army::get_id() const
{
    return army_id_;
}

Sprite& Army::get_sprite()
{
    return *sprite_;
}

const SDL_Rect& Army::get_sprite_rect() const
{
    return pos_;
}

void Army::get_sprite_base_center(glm::vec2& center_position)
{
    center_position = get_position();
}

float Army::scale()
{
    return current_scale_;
}

void Army::init_path()
{
    path_distance = movement_path.update_path_samples();
    time_path_will_take_ = path_distance / (common::kMovementPerHour1kmh * speed_);
    path_position_ = 0;
    delta_counter = 0;
    path_active = true;
    time_point = std::chrono::steady_clock::now();
}

void Army::populate()
{
    // Adds noteworthy characters to an army
}

void assign(ArmyType type, Amount amount) {}
