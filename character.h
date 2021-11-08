#pragma once
#include "movable.h"
#include "drawable.h"
#include "position.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>

namespace king
{
using CharacterID = std::string;
using Owner = std::string;

class Character : public MovableEntity
{

public:
    Character();
    ~Character();
    const CharacterID& get_id();
    void move(Origin origin) override;
protected:
    CharacterID character_id_;
    std::string name_;
};

} // namespace king