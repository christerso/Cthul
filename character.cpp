#include "character.h"

#include <boost/uuid/uuid_io.hpp>

using namespace king;

Character::Character(Position& pos)
    : position_(pos)
    , character_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
}

const CharacterID& Character::get_id()
{
    return character_id_;
}
