#pragma once
#include "position.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>

namespace king
{
using CharacterID = std::string;
using Owner = std::string;

class Character
{

public:
    explicit Character(Position& pos);
    const CharacterID& get_id();

private:
    Position position_;
    CharacterID character_id_;
    std::string name_;
};

} // namespace king