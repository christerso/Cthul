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
    : m_owner(owner.get_id())
    , m_army_id(to_string(boost::uuids::random_generator()())), m_sprite(sprite),
    m_army_size(army_size)
{
    set_position(pos);
    set_velocity(5.0f);
    LOG(INFO) << "created army: " << m_army_id;
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
    m_current_path_position += common::kMovementPerSecondPer1Kmh * speed_;
    m_path_step_position = static_cast<size_t>(m_current_path_position / (path_distance / static_cast<double>(path.size())));
    std::cout << "Path Step:" << m_path_step_position << std::endl;
    m_position = path[m_path_step_position];

    // take the path length in km and divide it by speed
    const auto path_update = path_distance - m_current_path_position;
    m_time_path_will_take = path_update / (common::kMovementPerHour1kmh * speed_);
    // difference between the current path start point and the current position
    auto diff = glm::distance2(m_position, path[m_path_position]);

    // get position to measure how far we've got

    if (m_path_position >= path.size())
    {
        path_active = false;
        movement_path.clear_path();
        return;
    }

    // check if destination reached
    const auto distance = glm::distance2(m_position, path[m_path_position]);
    if (distance < 1.0f && m_path_position <= path.size())
    {
        ++m_path_position;
    }
}

void Army::draw(SDL_Renderer* renderer)
{
    auto& pos = get_position();
    SDL_Rect origin{ static_cast<int>(pos.x), static_cast<int>(pos.y), m_sprite->source_rect.w, m_sprite->source_rect.h };
    scale_object(m_center, origin, pos_, m_current_scale);
    //SDL_RenderDrawRect(renderer, &pos_);
    SDL_RenderCopyEx(renderer, m_sprite->texture, &m_sprite->source_rect, &pos_, 0, &m_center, SDL_FLIP_NONE);
}

const ArmyID& Army::get_id() const
{
    return m_army_id;
}

Sprite& Army::get_sprite()
{
    return *m_sprite;
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
    return m_current_scale;
}

void Army::init_path()
{
    path_distance = movement_path.update_path_samples();
    m_time_path_will_take = path_distance / (common::kMovementPerHour1kmh * speed_);
    m_path_position = 0;
    delta_counter = 0;
    path_active = true;
    time_point = std::chrono::steady_clock::now();
}

void Army::populate()
{
    // Adds noteworthy characters to an army
}

void assign(ArmyType type, Amount amount) {}
