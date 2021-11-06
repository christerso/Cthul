#include "army.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>
#include <map>
#include <array>


using namespace king;

Army::Army(Character& owner, Position& pos, Sprite* sprite, int army_size)
    : owner_(owner.get_id())
    , army_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
    sprite_ = sprite;
    army_size_ = army_size;
    LOG(INFO) << "created army: " << army_id_;
}

Army::~Army() {}


void Army::move(Origin origin)
{

}

void Army::draw(SDL_Renderer* renderer)
{
    auto hcenter = sprite_->source_rect.w / 2;
    Position pos = get_position();
    SDL_FRect dest{pos.pos_x, pos.pos_y, sprite_->source_rect.w /2, sprite_->source_rect.h / 2};
    LOG(INFO) << "W:" << dest.w << "H:" << dest.h << "x:" << dest.x << "y:" << dest.y;
    SDL_RenderCopyExF(renderer, sprite_->texture, &sprite_->source_rect, &dest, 0, nullptr, SDL_FLIP_NONE);
}

const ArmyID& Army::get_id() const
{
    return army_id_;
}

void Army::populate()
{
    // Adds noteworthy characters to an army
}

void assign(ArmyType type, Amount amount) {}