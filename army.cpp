#include "army.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>
#include <cmath>
#include <glm/gtx/norm.hpp>

using namespace king;

Army::Army(Character& owner, glm::vec2& pos, Sprite* sprite, int army_size)
    : owner_(owner.get_id())
    , army_id_(to_string(boost::uuids::random_generator()())), sprite_(sprite),
    army_size_(army_size)
{
    set_position(pos);
    set_velocity(5.0f);
    LOG(INFO) << "created army: " << army_id_;
}

Army::~Army() {}

// a call to move will only happen if there actually is a move to be done
void Army::move(Origin origin)
{
    auto& path = movement_path.get_path();

}

void Army::set_velocity(float velocity)
{
    velocity_ = velocity;
}

float Army::get_velocity() const
{
    return velocity_;
}

void Army::draw(SDL_Renderer* renderer)
{
    auto& pos = get_position();
    SDL_Rect origin{ static_cast<int>(pos.x), static_cast<int>(pos.y), sprite_->source_rect.w, sprite_->source_rect.h };
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

void Army::get_sprite_base_center(glm::vec2& center_position)
{
    center_position = get_position();
}

float Army::scale()
{
    return current_scale_;
}

void Army::init_path()
{
    movement_path.update_path_samples();
    path_position_ = 0;
    path_active = true;
}

void Army::populate()
{
    // Adds noteworthy characters to an army
}

void assign(ArmyType type, Amount amount) {}
