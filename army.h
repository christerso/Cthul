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

class Army : public MovableEntity
{
public:
    Army(Character& owner, Position& pos, Sprite* sprite, int army_size);
    ~Army();
   

    void move(Origin origin) override;
    void draw(SDL_Renderer* renderer) override;
    const ArmyID& get_id() const;
private:
    void update_map_symbol();
    void populate();
    Owner owner_; // id of owner, get_id() from army and character
    Origin origin_ = Origin::AI;
    ArmyID army_id_;
    Sprite* sprite_;
    std::vector<Position> waypoints_;
    int army_size_;
};
} // namespace king
