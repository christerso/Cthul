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

using Amount = int;

class Army : public MovableEntity, Drawable
{
public:
    Army(Character& owner, Position& pos, Sprite* sprite, int army_size);
    ~Army();
   

    void move(Origin origin) override;
    void draw(SDL_Renderer* renderer) override;
    const ArmyID& get_id() const;
    Sprite& get_sprite();
    SDL_Rect& get_sprite_rect();
private:
    void update_map_symbol();
    void populate();
    Owner owner_ {}; // id of owner, get_id() from army and character
    Origin origin_ = Origin::AI;
    ArmyID army_id_ {};
    Sprite* sprite_ = nullptr;
    std::vector<Position> waypoints_;
    int army_size_ {};
    SDL_Point center_ {};
    SDL_Rect pos_ {};
    int current_scale_ = 4;
};
} // namespace king
