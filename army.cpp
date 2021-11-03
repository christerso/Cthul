#include "army.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>

using namespace king;

Army::Army(Character& owner, Position& pos)
    : owner_(owner.get_id())
    , position_(pos)
    , army_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
    LOG(INFO) << "created army: " <<  army_id_;
}

Army::~Army()
{
}

void Army::move(Position pos)
{
    position_ = pos;
}

void Army::act(Action action, Position pos) {}

Position& Army::get_position()
{
    return position_;
}

ArmyID& Army::get_id()
{
    return army_id_;
}

void Army::populate()
{
}
