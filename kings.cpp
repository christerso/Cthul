// Chtuhl.cpp : Defines the entry point for the application.
//

#include "kings.h"
#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <thread_pool.hpp>
#include "threadmanager.h"
using namespace RM;
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
    int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        exit (-1);
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

void Kings::setup_resources()
{
    resources_.setup_initial_resources();
   // auto my_future = pool.submit(task to run, arguments);
}

void Kings::setup_kingdom()
{
    kingdom_ = std::make_unique<Kingdom>(Kingdom(thread_manager_));
}

void Kings::draw_sprites()
{
    // Iterate over all sprites needed rendering
    // TODO: Setup rules and instantiate beginning setup
    // TODO: First now, just draw 1 sprite and move it around
    SDL_FRect d = {0.f,0.f,100.f,120.f};
    for (auto const& [key, val] : resources_.get_entities()) {

        SDL_RenderCopyExF(renderer_, val.get()->texture, &val->source_rect, &d, 0, nullptr, SDL_FLIP_NONE);
    }
}

Kingdom& Kings::get_kingdom() const
{
    return *kingdom_;
}

void Kings::start()
{
    initialize_sdl2();
    create_window();
    setup_resources();
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
    SDL_QueryTexture(tex.texture, nullptr, nullptr, &map_width, &map_height);
    SDL_Rect position = {0, 0, map_width, map_height};
    const SDL_Rect camera_rect = {0, 0, position.w / 4, position.h / 4};
    // Handle events on queue
    // Get map width and height for the scrolling limits
    SDL_Event ev{};
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    int updated_delta = 0;
    int movement_time = 200;
    while (!thread_manager_.quit)
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
                }
                if (ev.button.button == SDL_BUTTON_MIDDLE)
                {
                    mouse_button_states_[MIDDLE] = true;
                }
                if (ev.button.button == SDL_BUTTON_RIGHT)
                {
                    mouse_button_states_[RIGHT] = true;
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
                if (ev.key.keysym.sym == SDLK_q)
                {
                    // quit
                    thread_manager_.quit = true;
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
                    thread_manager_.quit = true;
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
            default:
                break;
            }
        }
        draw_rect = {position.x, position.y, position.w - camera_rect.w, position.h - camera_rect.h};

        // clear screen
        SDL_RenderClear(renderer_);
        SDL_RenderCopy(renderer_, tex.texture, &draw_rect, NULL);
        draw_sprites();
        // Render texture to screen
        // SDL_RenderCopy(renderer_, tex, NULL, &a);
        SDL_RenderPresent(renderer_);
    }
    if (delta < delay_time)

    {
        SDL_Delay(delay_time - delta);
    }
}
