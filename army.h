#pragma once

#include "character.h"
#include "common.h"
#include "entity.h"
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

struct Speeds
{
    double kGuarding = 0.0;
    double kWalking = 5.0;
    double kForcedMove = 10.0;
    double kRun = 15.0;
};

using Amount = int;
using TimeSample = std::chrono::time_point<std::chrono::milliseconds>;
// TODO: add original destination so the army can resume it?
class Army : public Entity, Drawable, Movable
{
public:
    Army(Character& owner, glm::vec2& pos, Sprite* sprite, int army_size);
    virtual ~Army();
    void move(Origin origin) override;
    void draw(SDL_Renderer* renderer) override;
    const ArmyID& get_id() const;
    Sprite& get_sprite();
    const SDL_Rect& get_sprite_rect() const;
    void get_sprite_base_center(glm::vec2& center_position);
    void init_path();
    Path movement_path;
    double path_distance;
    bool path_active = false;
    int delta_counter {};
    std::chrono::steady_clock::time_point time_point;
private:
    double calculate_path_length();
    size_t m_path_step_position {};
    size_t m_path_position;
    double m_current_path_position {};
    double m_time_path_will_take {};
    void update_map_symbol();
    void populate();
    Owner m_owner {}; // id of owner, get_id() from army and character
    Origin m_origin = Origin::kEntity;
    ArmyID m_army_id {};
    Sprite* m_sprite = nullptr;
    std::vector<glm::vec2> m_waypoints;
    int m_army_size {};
    SDL_Point m_center {};
    SDL_Rect pos_ {};
    float m_current_scale = .4f;
    int m_current_path_waypoint;
    const float m_epsilon = 100.0f;
};
} // namespace king
