#include "input.h"

#include <glog/logging.h>

using namespace king;

Input::Input()
{

    for (int i = 0; i < 3; i++)
    {
        mouse_button_states_.push_back(false);
    }
    // setup position for map
}

Input::~Input()
{
    LOG(INFO) << "Input destructor called";
}

const SDL_Rect& Input::get_camera() const
{
    return camera_;
}

float Input::get_scale() const
{
    return scale_;
}

void Input::set_camera(SDL_Rect& camera)
{
    camera_ = camera;
}

bool Input::get_left_mouse_button_state()
{
    std::scoped_lock lock(input_mtx);
    return mouse_button_states_[kLeft];
}

bool Input::get_right_mouse_button_state()
{
    std::scoped_lock lock(input_mtx);
    return mouse_button_states_[kRight];
}

MousePosition& Input::left_mouse_button_entry()
{
    return current_left_mouse_entry_;
}

MousePosition& Input::right_mouse_button_entry()
{
    return current_right_mouse_entry_;
}

MousePosition& Input::scroll_wheel_mouse_position()
{
    return scroll_wheel_mouse_position_;
}

void Input::reset_mouse_button_state(MouseButtons button)
{
    mouse_button_states_[button] = false;
}

void Input::setup_world_data(int width, int height)
{
    auto val = mouse_button_states_.begin();
    ++val;
    world_width_ = width;
    world_height_ = height;
}

bool Input::input_loop()
{
    while (SDL_PollEvent(&sdl_event_) != 0)
    {
        switch (sdl_event_.type)
        {

        case SDL_MOUSEWHEEL:
        {
            std::scoped_lock lock{ input_mtx };
            SDL_GetMouseState(&scroll_wheel_mouse_position_.x_pos, &scroll_wheel_mouse_position_.y_pos);

            if (sdl_event_.wheel.y > 0) // scroll up
            {
                if (scale_ <= 3.f)
                {
                }
            }
            //else if (sdl_event_.wheel.y < 0) // scroll down
            //{
            //    if (scale_ >= 0.5f)
            //    {
            //        scale_ -= 0.001f;
            //    }
            //}
            //if (camera_.w < world_width_)
            //{
            //    camera_.w *= scale_;
            //}
            //if (camera_.w >= world_width_)
            //{
            //    camera_.w = world_width_;
            //}

            //if (camera_.h < world_height_)
            //{
            //    camera_.h *= scale_;
            //}
            //if (camera_.h >= world_height_)
            //{
            //    camera_.h = world_height_;
            //}

            //if (camera_.x > 0)
            //{
            //    
            //}

        }
        case SDL_MOUSEBUTTONDOWN:
        {
            if (sdl_event_.button.button == SDL_BUTTON_LEFT)
            {
                std::scoped_lock lock{ input_mtx };
                mouse_button_states_[kLeft] = true;
                SDL_GetMouseState(&current_left_mouse_entry_.x_pos, &current_left_mouse_entry_.y_pos);
            }
            if (sdl_event_.button.button == SDL_BUTTON_MIDDLE)
            {
                std::scoped_lock lock{ input_mtx };
                mouse_button_states_[kMiddle] = true;
                SDL_GetMouseState(&current_middle_mouse_entry_.x_pos, &current_middle_mouse_entry_.y_pos);
            }
            if (sdl_event_.button.button == SDL_BUTTON_RIGHT)
            {
                std::scoped_lock lock{ input_mtx };
                mouse_button_states_[kRight] = true;
                SDL_GetMouseState(&current_right_mouse_entry_.x_pos, &current_right_mouse_entry_.y_pos);
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if (sdl_event_.button.button == SDL_BUTTON_LEFT)
            {
                std::scoped_lock lock{ input_mtx };
                mouse_button_states_[kLeft] = false;
            }
            if (sdl_event_.button.button == SDL_BUTTON_MIDDLE)
            {
                std::scoped_lock lock{ input_mtx };
                mouse_button_states_[kMiddle] = false;
            }
            if (sdl_event_.button.button == SDL_BUTTON_RIGHT)
            {
                std::scoped_lock lock{ input_mtx };
                mouse_button_states_[kRight] = false;
            }
            break;
        }
        /* Look for a keypress */
        case SDL_KEYDOWN:
        {
            if (sdl_event_.key.keysym.sym == SDLK_q)
            {
                // quit
                return false;
            }
            if (sdl_event_.key.keysym.sym == SDLK_LCTRL)
            {
                // when lctrl is down and left mouse button is clicked after having selected an army, waypoints are
                // queued
                left_ctrl_down_ = true;
                LOG(INFO) << "LCTRL down";
            }
            if (sdl_event_.key.keysym.sym == SDLK_LALT)
            {
                left_alt_down_ = true;
                LOG(INFO) << "LALT down";
            }
            if (sdl_event_.key.keysym.sym == SDLK_LSHIFT)
            {
                left_shift_down_ = true;
                LOG(INFO) << "LSHIFT down";
            }
            if (sdl_event_.key.keysym.sym == SDLK_p)
            {
                if (left_alt_down_) {
                    show_astar_path_ = true;
                }
            }

            if (sdl_event_.key.keysym.sym == SDLK_w || sdl_event_.key.keysym.sym == SDLK_UP)
            {
                // move north
                if (camera_.y > 0)
                {
                    camera_.y -= scroll_speed_;
                }
                if (camera_.y < 0)
                {
                    camera_.y = 0;
                }
            }
            if (sdl_event_.key.keysym.sym == SDLK_d || sdl_event_.key.keysym.sym == SDLK_RIGHT)
            {
                // move east
                if (camera_.x < world_width_ - camera_.w)
                {
                    camera_.x += scroll_speed_;
                }
                else if (camera_.x > world_width_ - camera_.w)
                {
                    camera_.x = world_width_ - camera_.w;
                }
            }
            if (sdl_event_.key.keysym.sym == SDLK_a || sdl_event_.key.keysym.sym == SDLK_LEFT)
            {
                // move west
                if (camera_.x > 0)
                {
                    camera_.x -= scroll_speed_;
                }
                else if (camera_.x < 0)
                {
                    camera_.x = 0;
                }
            }
            if (sdl_event_.key.keysym.sym == SDLK_s || sdl_event_.key.keysym.sym == SDLK_DOWN)
            {
                // move south
                if (camera_.y < world_height_ - camera_.h)
                {
                    camera_.y += scroll_speed_;
                }
                if (camera_.y > world_height_ - camera_.h)
                {
                    camera_.y = world_height_ - camera_.h;
                }
            }
            if (sdl_event_.type == SDL_QUIT)
            {
                return false;
            }
            else if (sdl_event_.type == SDL_TEXTINPUT)
            {
                LOG(INFO) << sdl_event_.text.text << std::endl;
            }
            else if (sdl_event_.type == SDL_MOUSEMOTION)
            {
                // delta_x_ += sdl_event_.motion.x;
                // delta_y_ += sdl_event_.motion.y;
            }
            break;
        }
        case SDL_KEYUP:
        {
            if (sdl_event_.key.keysym.sym == SDLK_LCTRL)
            {
                left_ctrl_down_ = false;
                LOG(INFO) << "LCTRL up";
            }
            if (sdl_event_.key.keysym.sym == SDLK_LALT)
            {
                left_alt_down_ = false;
                LOG(INFO) << "LALT up";
            }

            if (sdl_event_.key.keysym.sym == SDLK_LSHIFT)
            {
                left_shift_down_ = false;
                LOG(INFO) << "LSHIFT up";
            }

            if (sdl_event_.key.keysym.sym == SDLK_p)
            {
                if (left_alt_down_) {
                    show_astar_path_ = false;
                }
            };
            break;
        }
        default:
            break;
        }
    }
    return true;
}

void Input::append_left_mouse_input(MousePosition& entry)
{
    current_left_mouse_entry_ = entry;
}

void Input::append_right_mouse_input(MousePosition& entry)
{
    current_right_mouse_entry_ = entry;
}

bool Input::show_astar_path() const
{
    return show_astar_path_;
}

const SelectedArmies& Input::get_selected_armies() const
{
    return selected_armies_;
}

void Input::add_selected_army(const ArmyID& army)
{
    selected_armies_.push_back(army);
}

void Input::clear_selected_armies()
{
    if (!left_ctrl_down_) {
        selected_armies_.clear();
    }
}
