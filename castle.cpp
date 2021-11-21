#include "castle.h"
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>

using namespace king;

Castle::Castle(common::Position& pos)
    : position_(pos)
    , castle_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
}

Castle::~Castle()
{
    LOG(INFO) << "Castle destructor called";
}

const common::Position& Castle::get_position() const
{
    return position_;
}

const CastleID& Castle::get_id() const
{
    return castle_id_;
}

Owner Castle::get_owner() const
{
    return owner_;
}

void Castle::set_owner(const CharacterID& owner)
{
    owner_ = owner;
}
