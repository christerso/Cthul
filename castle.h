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
    explicit Castle(glm::vec2& pos);
    virtual ~Castle();
    const glm::vec2& get_position() const;
    const CastleID& get_id() const;
    Owner get_owner() const;
    void set_owner(const CharacterID& owner);
private:
    Owner owner_;
    glm::vec2 position_;
    CastleID castle_id_;
    std::string castle_name_;
};

} // namespace king