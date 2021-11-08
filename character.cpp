#include "character.h"

#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>

using namespace king;

Character::Character()
    : character_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
}

Character::~Character()
{
    LOG(INFO) << "Character destructor called";
}
const CharacterID& Character::get_id()
{
    return character_id_;
}

void Character::move(Origin origin)
{
}
