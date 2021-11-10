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
    set_position(pos);
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
    auto &[x, y] = get_position();
    SDL_Rect origin{static_cast<int>(x), static_cast<int>(y), sprite_->source_rect.w, sprite_->source_rect.h};
    scale_object(center_, origin, pos_, current_scale_);
    SDL_RenderCopyEx(renderer, sprite_->texture, &sprite_->source_rect, &pos_, 0, &center_, SDL_FLIP_NONE);
}

const ArmyID& Army::get_id() const
{
    return army_id_;
}

Sprite& Army::get_sprite()
{
    return *sprite_;
}

SDL_Rect& Army::get_sprite_rect()
{
    return pos_;
}

void Army::populate()
{
    // Adds noteworthy characters to an army
}

void assign(ArmyType type, Amount amount) {}