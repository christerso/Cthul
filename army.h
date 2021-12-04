#pragma once

#include "character.h"
#include "common.h"
#include "movable.h"
#include "path.h"
#include "resourcemanager.h"
#include "timer.h"

#include <mutex>
#include <string>
#include <SDL_render.h>
#include <glm/vec2.hpp>
#include <chrono>

namespace king
{
using ArmyID = std::string;

enum class ArmyType
{
    HUMAN_SWORDMAN,
    HUMAN_SPEARMAN_RIDER,
    HUMAN_AXEMAN_RIDER,
    HUMAN_SHIELDMAN_RIDER
};

enum class ArmyState
{
    kGuarding,
    kWalking,
    kForcedMove,
    kRun
};

using Amount = int;
using TimeSample = std::chrono::time_point<std::chrono::milliseconds>;
// TODO: add original destination so the army can resume it?
class Army : public MovableEntity, Drawable
{
public:
    Army(Character& owner, glm::vec2& pos, Sprite* sprite, int army_size);
    virtual ~Army();
    void move(Origin origin) override;
    void set_velocity(float velocity);
    float get_velocity() const;
    void draw(SDL_Renderer* renderer) override;
    const ArmyID& get_id() const;
    Sprite& get_sprite();
    const SDL_Rect& get_sprite_rect() const;
    void get_sprite_base_center(glm::vec2& center_position);
    float scale();
    void init_path();
    Path movement_path;
    double path_distance;
    bool path_active = false;
    int delta_counter {};
    std::chrono::steady_clock::time_point time_point;
private:
    double calculate_path_length();
    size_t path_step_position_ {};
    size_t path_position_;
    double current_path_position_ {};
    double time_path_will_take_ {};
    void update_map_symbol();
    void populate();
    Owner owner_ {}; // id of owner, get_id() from army and character
    Origin origin_ = Origin::AI;
    ArmyID army_id_ {};
    double speed_ {5.0f}; // 5km/h
    Sprite* sprite_ = nullptr;
    std::vector<glm::vec2> waypoints_;
    int army_size_ {};
    SDL_Point center_ {};
    SDL_Rect pos_ {};
    float current_scale_ = .4f;
    int current_path_waypoint;
    const float epsilon_ = 100.0f;
};
} // namespace king
