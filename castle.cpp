#include "castle.h"
#include <boost/uuid/uuid_io.hpp>

using namespace king;

Castle::Castle(Position& pos)
    : position_(pos)
    , castle_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
}

Castle::~Castle()
{
}

Position& Castle::get_position()
{
    return position_;
}

const CastleID& Castle::get_id()
{
    return castle_id_;
}

const Owner Castle::get_owner() const
{
    return owner_;
}

void Castle::set_owner(const CharacterID owner)
{
    owner_ = owner;
}
