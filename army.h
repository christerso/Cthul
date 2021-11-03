#pragma once

#include "character.h"
#include "common.h"
#include "movable.h"
#include "threadmanager.h"

#include <string>
#include <boost/uuid/uuid.hpp>

namespace king
{
using ArmyID = std::string;

class Army : public Movable
{
public:
    Army(Character& owner, Position& pos);
    ~Army();
    void move(Position pos) override;
    void act(Action action, Position pos = {}); // perform an action (move, guard, attack)
    Position& get_position();
    ArmyID& get_id();
private:
    void populate();
    Owner owner_;
    Position position_;
    ArmyID army_id_;
};
} // namespace king
