#pragma once

#include "character.h"
#include "common.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>


namespace king
{

using CastleID = std::string;
class Castle
{
public:
    explicit Castle(common::Position& pos);
    virtual ~Castle();
    const common::Position& get_position() const;
    const CastleID& get_id() const;
    Owner get_owner() const;
    void set_owner(const CharacterID& owner);
private:
    Owner owner_;
    common::Position position_;
    CastleID castle_id_;
    std::string castle_name_;
};

} // namespace king