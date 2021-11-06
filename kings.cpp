// Chtuhl.cpp : Defines the entry point for the application.
//

#include "kings.h"
#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <thread_pool.hpp>

using namespace king;

Kings::Kings()
{
    for (int i = 0; i < 3; i++)
    {
        mouse_button_states_.push_back(false);
    }
}

Kings::~Kings()
{
    delete &kingdom_;
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void Kings::initialize_sdl2()
{
    if ((SDL_Init(SDL_INIT_EVERYTHING) == -1))
    {
        LOG(ERROR) << "Could not initialize SDL: {}", SDL_GetError();
        exit(-1);
    }
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        exit(-1);
    }
}

void Kings::shutdown_sdl2()
{
    window_ = nullptr;
    IMG_Quit();
    SDL_Quit();

    LOG(INFO) << "SDL2 is exiting..." << std::endl;
}

void Kings::create_window()
{
    // Create window
    window_ = SDL_CreateWindow("Cthul", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1200, SDL_WINDOW_OPENGL);
    if (window_ == nullptr)
    {
        LOG(ERROR) << fmt::format("Window could not be created !SDL_Error: {}", SDL_GetError());
        shutdown_sdl2();
    }
    else
    {
        // Get window surface
        SDL_SetWindowBordered(window_, SDL_FALSE);
        screen_surface_ = SDL_GetWindowSurface(window_);
    }
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == nullptr)
    {
        LOG(ERROR) << fmt::format("renderer could not be created! SDL Error: {}", SDL_GetError());
    }
    resources_.set_renderer(renderer_);
    // initialize renderer color
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
}

void Kings::setup_kingdom()
{
    resources_.setup_initial_resources();
    kingdom_.set_input_handler(&input_);
    kingdom_.set_astar_data(resources_.get_astar_data());
    kingdom_.start_threads();
}

Kingdom& Kings::get_kingdom()
{
    return kingdom_;
}

void Kings::start()
{
    initialize_sdl2();
    create_window();
    setup_kingdom();
    start_input_loop();
    shutdown_sdl2();
}

void Kings::start_input_loop()
{
    int width{4098};
    const int height{4098};
    const int FPS = 144;
    const int scroll_speed = 50;
    const int delay_time = static_cast<int>(1000.0f / FPS);
    // Event handler
    int previous_time{};
    int current_time{};
    int delta = {};
    int delta_x = {};
    int delta_y = {};
    int map_width = {};
    int map_height = {};
    SDL_Rect draw_rect{};
    const auto tex = resources_.get_sprite("map");
    SDL_QueryTexture(tex->texture, nullptr, nullptr, &map_width, &map_height);
    SDL_Rect position = {0, 0, map_width, map_height};
    const SDL_Rect camera_rect = {0, 0, position.w / 4, position.h / 4};
    // Handle events on queue
    // Get map width and height for the scrolling limits
    SDL_Event ev{};
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    int updated_delta = 0;
    int movement_time = 200;

    auto exit = false;
    while (true)
    {
        previous_time = current_time;
        current_time = SDL_GetTicks();
        delta = current_time - previous_time;
        updated_delta += delta;

        while (SDL_PollEvent(&ev) != 0)
        {
            switch (ev.type)
            {
            case SDL_MOUSEBUTTONDOWN:
            {
                if (ev.button.button == SDL_BUTTON_LEFT)
                {
                    mouse_button_states_[LEFT] = true;
                    MousePosition entry;
                    SDL_GetMouseState(&entry.x_pos, &entry.y_pos);
                    DLOG(INFO) << "Source Mouse X: " << entry.x_pos << " Y: " << entry.y_pos;
                    // this is a temporary buffer to be processed by the input thread
                    input_.current_left_mouse_entry = entry;
                }
                if (ev.button.button == SDL_BUTTON_MIDDLE)
                {
                    mouse_button_states_[MIDDLE] = true;
                }
                if (ev.button.button == SDL_BUTTON_RIGHT)
                {
                    mouse_button_states_[RIGHT] = true;
                    MousePosition entry;
                    DLOG(INFO) << "Target Mouse X: " << entry.x_pos << " Y: " << entry.y_pos;
                    SDL_GetMouseState(&entry.x_pos, &entry.y_pos);
                    // this is a temporary buffer to be processed by the input thread
                    input_.current_right_mouse_entry = entry;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (ev.button.button == SDL_BUTTON_LEFT)
                {
                    mouse_button_states_[LEFT] = false;
                }
                break;
            }
            /* Look for a keypress */
            case SDL_KEYDOWN:
            {
                if (ev.key.keysym.sym == SDLK_q)
                {
                    // quit
                    exit = true;
                }
                if (ev.key.keysym.sym == SDLK_w || ev.key.keysym.sym == SDLK_UP)
                {
                    // move up
                    if (position.y > 0)
                    {
                        position.y -= scroll_speed;
                    }
                    if (position.y < 0)
                    {
                        position.y = 0;
                    }
                }
                if (ev.key.keysym.sym == SDLK_LCTRL)
                {
                    // when lctrl is down and left mouse button is clicked after having selected an army, waypoints are
                    // queued
                    input_.left_ctrl_down = true;
                    LOG(INFO) << "LCTRL down";
                }
                if (ev.key.keysym.sym == SDLK_LALT)
                {
                    input_.left_alt_down = true;
                    LOG(INFO) << "LALT down";
                }
                if (ev.key.keysym.sym == SDLK_LSHIFT)
                {
                    input_.left_shift_down = true;
                    LOG(INFO) << "LSHIFT down";
                }
                if (ev.key.keysym.sym == SDLK_a || ev.key.keysym.sym == SDLK_LEFT)
                {
                    // move left;
                    if (position.x > 0)
                    {
                        position.x -= scroll_speed;
                    }
                    if (position.x < 0)
                    {
                        position.x = 0;
                    }
                }
                if (ev.key.keysym.sym == SDLK_d || ev.key.keysym.sym == SDLK_RIGHT)
                {
                    // move right
                    if (position.x < camera_rect.w)
                    {
                        position.x += scroll_speed;
                    }
                    if (position.x > camera_rect.w)
                    {
                        position.x = camera_rect.w;
                    }
                }
                if (ev.key.keysym.sym == SDLK_s || ev.key.keysym.sym == SDLK_DOWN)
                {
                    // move down
                    if (position.y < camera_rect.h)
                    {
                        position.y += scroll_speed;
                    }
                    if (position.y > camera_rect.h)
                    {
                        position.y = camera_rect.h;
                    }
                }
                if (ev.type == SDL_QUIT)
                {
                    exit = true;
                }
                else if (ev.type == SDL_TEXTINPUT)
                {
                    LOG(INFO) << ev.text.text << std::endl;
                }
                else if (ev.type == SDL_MOUSEMOTION)
                {
                    delta_x += ev.motion.x;
                    delta_y += ev.motion.y;
                }
                break;
            }
            case SDL_KEYUP:
            {
                if (ev.key.keysym.sym == SDLK_LCTRL)
                {
                    input_.left_ctrl_down = false;
                    LOG(INFO) << "LCTRL up";
                }
                if (ev.key.keysym.sym == SDLK_LALT)
                {
                    input_.left_alt_down = false;
                    LOG(INFO) << "LALT up";
                }
                if (ev.key.keysym.sym == SDLK_LSHIFT)
                {
                    input_.left_shift_down = false;
                    LOG(INFO) << "LSHIFT up";
                }
            }
            default:
                break;
            }
            if (exit)
            {
                break;
            }
        }
        if (exit)
        {
            break;
        }
        draw_rect = {position.x, position.y, position.w - camera_rect.w, position.h - camera_rect.h};

        // clear screen
        SDL_RenderClear(renderer_);
        SDL_RenderCopy(renderer_, tex->texture, &draw_rect, NULL);
        kingdom_.draw_sprites(renderer_);
        // Render texture to screen
        // SDL_RenderCopy(renderer_, tex, NULL, &a);
        SDL_RenderPresent(renderer_);
        if (delta < delay_time)

        {
            SDL_Delay(delay_time - delta);
        }
    }
}
