#include "character.h"

#include <boost/uuid/uuid_io.hpp>

using namespace king;

Character::Character()
    : character_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
}

Character::~Character()
{}
const CharacterID& Character::get_id()
{
    return character_id_;
}

void Character::move(Origin origin)
{
}

void Character::draw(SDL_Renderer* renderer)
{
}
