#pragma once

#include "character.h"
#include "common.h"
#include "position.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>


namespace king
{

using CastleID = std::string;
class Castle
{
public:
    explicit Castle(Position& pos);
    virtual ~Castle();
    Position& get_position();
    const CastleID& get_id();
    const Owner get_owner() const;
    void set_owner(const CharacterID owner);
private:
    Owner owner_;
    Position position_;
    CastleID castle_id_;
    std::string castle_name_;
};

} // namespace king