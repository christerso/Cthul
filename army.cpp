#include "army.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>

using namespace king;

Army::Army(Character& owner, common::Position& pos, Sprite* sprite, int army_size)
    : owner_(owner.get_id())
    , army_id_(boost::uuids::to_string(boost::uuids::random_generator()())), sprite_(sprite),
    army_size_(army_size)
{
    set_position(pos);

    LOG(INFO) << "created army: " << army_id_;
}

Army::~Army() {}


void Army::move(Origin origin)
{

}

void Army::draw(SDL_Renderer* renderer)
{
    auto& [x, y] = get_position();
    SDL_Rect origin{ x, y, sprite_->source_rect.w, sprite_->source_rect.h };
    scale_object(center_, origin, pos_, current_scale_);
    //SDL_RenderDrawRect(renderer, &pos_);
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

const SDL_Rect& Army::get_sprite_rect() const
{
    return pos_;
}

void Army::get_sprite_base_center(SDL_Point& center_position)
{
    auto& [x, y] = get_position();

    center_position = { x, y };
}

float king::Army::scale()
{
    return current_scale_;
}

void Army::set_movement_path(AstarMovementPath& path)
{
    int movement_path = 0;

    /*
     * Todo: for each entry entry in the movement path we can generate a 2d vector
     * Steps:
     * Iterate over the path, take the int, convert it to the pixel coords of the square it represents.
     * Convert it to it's screen coordinates
     * Store it as a movement path.
     *
     */

    for (auto& entry : path)
    {

    }
}

AstarMovementPath& Army::get_movement_path()
{
    return movement_path_;
}

void Army::populate()
{
    // Adds noteworthy characters to an army
}

void assign(ArmyType type, Amount amount) {}
