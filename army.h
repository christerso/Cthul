#pragma once

#include "character.h"
#include "common.h"
#include "movable.h"
#include "resourcemanager.h"
#include "timer.h"

#include <mutex>
#include <string>
#include <SDL_render.h>
#include <glm/vec2.hpp>

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
    common::Path movement_path;
    bool path_active = false;
private:
    size_t path_step_position_ {};
    float path_position_;

    void update_map_symbol();
    void populate();
    Owner owner_ {}; // id of owner, get_id() from army and character
    Origin origin_ = Origin::AI;
    ArmyID army_id_ {};
    Sprite* sprite_ = nullptr;
    std::vector<glm::vec2> waypoints_;
    int army_size_ {};
    SDL_Point center_ {};
    SDL_Rect pos_ {};
    float current_scale_ = .4f;
    int current_path;
    int current_path_waypoint;
    const float epsilon_ = 100.0f;
};
} // namespace king
