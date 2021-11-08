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

const SDL_Rect& Input::get_position() const
{
    return position_;
}

void Input::setup_world_data(int width, int height)
{
    world_width_ = width;
    world_height_ = height;
    position_.w = width;
    position_.h = height;
    camera_ = {0, 0, width / 3, height / 3};
}

bool Input::input_loop()
{
    while (SDL_PollEvent(&sdl_event_) != 0)
    {
        switch (sdl_event_.type)
        {
        case SDL_MOUSEBUTTONDOWN:
        {
            if (sdl_event_.button.button == SDL_BUTTON_LEFT)
            {
                mouse_button_states_[kLeft] = true;
                MousePosition entry;
                SDL_GetMouseState(&entry.x_pos, &entry.y_pos);
                DLOG(INFO) << "Source Mouse X: " << entry.x_pos << " Y: " << entry.y_pos;
                // this is a temporary buffer to be processed by the input thread
                current_left_mouse_entry = entry;
            }
            if (sdl_event_.button.button == SDL_BUTTON_MIDDLE)
            {
                mouse_button_states_[kMiddle] = true;
            }
            if (sdl_event_.button.button == SDL_BUTTON_RIGHT)
            {
                mouse_button_states_[kRight] = true;
                MousePosition entry;
                DLOG(INFO) << "Target Mouse X: " << entry.x_pos << " Y: " << entry.y_pos;
                SDL_GetMouseState(&entry.x_pos, &entry.y_pos);
                // this is a temporary buffer to be processed by the input thread
                current_right_mouse_entry = entry;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if (sdl_event_.button.button == SDL_BUTTON_LEFT)
            {
                mouse_button_states_[MouseButtons::kLeft] = false;
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
            if (sdl_event_.key.keysym.sym == SDLK_w || sdl_event_.key.keysym.sym == SDLK_UP)
            {
                // move up
                if (position_.y > 0)
                {
                    position_.y -= scroll_speed_;
                }
                if (position_.y < 0)
                {
                    position_.y = 0;
                }
            }
            if (sdl_event_.key.keysym.sym == SDLK_LCTRL)
            {
                // when lctrl is down and left mouse button is clicked after having selected an army, waypoints are
                // queued
                left_ctrl_down = true;
                LOG(INFO) << "LCTRL down";
            }
            if (sdl_event_.key.keysym.sym == SDLK_LALT)
            {
                left_alt_down = true;
                LOG(INFO) << "LALT down";
            }
            if (sdl_event_.key.keysym.sym == SDLK_LSHIFT)
            {
                left_shift_down = true;
                LOG(INFO) << "LSHIFT down";
            }
            if (sdl_event_.key.keysym.sym == SDLK_a || sdl_event_.key.keysym.sym == SDLK_LEFT)
            {
                // move left;
                if (position_.x > 0)
                {
                    position_.x -= scroll_speed_;
                }
                if (position_.x < 0)
                {
                    position_.x = 0;
                }
            }
            if (sdl_event_.key.keysym.sym == SDLK_d || sdl_event_.key.keysym.sym == SDLK_RIGHT)
            {
                // move right

                if (position_.x < world_width_ - camera_.w * 1.5)
                {
                    position_.x += scroll_speed_;
                }
                if (position_.x > world_width_ - camera_.w * 1.5)
                {
                    position_.x = world_width_ - camera_.w * 1.5;
                }
            }
            if (sdl_event_.key.keysym.sym == SDLK_s || sdl_event_.key.keysym.sym == SDLK_DOWN)
            {
                // move down
                if (position_.y < camera_.h)
                {
                    position_.y += scroll_speed_;
                }
                if (position_.y > camera_.h)
                {
                    position_.y = camera_.h;
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
                //delta_x_ += sdl_event_.motion.x;
                //delta_y_ += sdl_event_.motion.y;
            }
            break;
        }
        case SDL_KEYUP:
        {
            if (sdl_event_.key.keysym.sym == SDLK_LCTRL)
            {
                left_ctrl_down = false;
                LOG(INFO) << "LCTRL up";
            }
            if (sdl_event_.key.keysym.sym == SDLK_LALT)
            {
                left_alt_down = false;
                LOG(INFO) << "LALT up";
            }
            if (sdl_event_.key.keysym.sym == SDLK_LSHIFT)
            {
                left_shift_down = false;
                LOG(INFO) << "LSHIFT up";
            }
        }
        default:
            break;
        }
    }
    return true;
}

void Input::append_left_mouse_input(MousePosition& entry)
{
    current_left_mouse_entry = entry;
}

void Input::append_right_mouse_input(MousePosition& entry)
{
    current_right_mouse_entry = entry;
}
