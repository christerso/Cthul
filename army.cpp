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

void Army::draw(SDL_Rect& position, SDL_Renderer* renderer)
{
    SDL_Point center{sprite_->source_rect.w / 2, sprite_->source_rect.h / 2};
    Position p = get_position();
    SDL_Rect pos {p.pos_x, p.pos_y, sprite_->source_rect.w / 6, sprite_->source_rect.h / 4};
    pos.x = pos.x - pos.w / 2;
    pos.y = pos.y - pos.h;
    SDL_RenderCopyEx(renderer, sprite_->texture, &sprite_->source_rect, &pos, 0, &center, SDL_FLIP_NONE);
    //Position p {pos.pos_x - position.x, pos.pos_y - position.y};
    //set_position();
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