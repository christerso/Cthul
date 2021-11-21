#pragma once

#include "character.h"
#include "common.h"
#include "movable.h"
#include "resourcemanager.h"
#include <string>
#include <SDL_render.h>

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
using AstarMovementPath = std::vector<int>;
class Army : public MovableEntity, Drawable
{
public:
    Army(Character& owner, common::Position& pos, Sprite* sprite, int army_size);
    virtual ~Army();
    void move(Origin origin) override;
    void draw(SDL_Renderer* renderer) override;
    const ArmyID& get_id() const;
    Sprite& get_sprite();
    const SDL_Rect& get_sprite_rect() const;
    void get_sprite_base_center(SDL_Point& center_position);
    float scale();
    void set_movement_path(AstarMovementPath& path);
    AstarMovementPath& get_movement_path();
    common::BezierPath bezier_path;
private:
    void update_map_symbol();
    void populate();
    Owner owner_ {}; // id of owner, get_id() from army and character
    Origin origin_ = Origin::AI;
    ArmyID army_id_ {};
    Sprite* sprite_ = nullptr;
    std::vector<common::Position> waypoints_;
    int army_size_ {};
    SDL_Point center_ {};
    SDL_Rect pos_ {};
    float current_scale_ = .4f;
    AstarMovementPath movement_path_;
    int current_path;
    int current_path_waypoint;
    const float epsilon_ = 5.0f;
};
} // namespace king
