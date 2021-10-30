// Chtuhl.cpp : Defines the entry point for the application.
//

#include "Cthul.h"
#include <SDL2/SDL.h>
#include <fmt/format.h>

using namespace RM;

Cthul::Cthul()
{
    for (int i = 0; i < 3; i++)
    {
        mouse_button_states_.push_back(false);
    }
}

Cthul::~Cthul()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void Cthul::initialize_SDL2()
{
    if ((SDL_Init(SDL_INIT_EVERYTHING) == -1))
    {
        LOG(ERROR) << "Could not initialize SDL: {}", SDL_GetError();
        exit(-1);
    }
}

void Cthul::shutdown_SDL2()
{
    window_ = nullptr;

    SDL_Quit();

    LOG(INFO) << "SDL2 is exiting..." << std::endl;
}

void Cthul::create_window()
{

    // Create window
    window_ = SDL_CreateWindow("Cthul", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    if (window_ == nullptr)
    {
        LOG(ERROR) << fmt::format("Window could not be created !SDL_Error: {}", SDL_GetError());
        shutdown_SDL2();
    }
    else
    {
        // Get window surface
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

void Cthul::setup_resources()
{
    resources_.setup_initial_resources();
}

void Cthul::start_input_loop()
{
    int width{2048};
    int height{2048};
    bool running = true;
    const int FPS = 144;
    const int scrollSpeed = 50;
    const int delay_time = static_cast<int>(1000.0f / FPS);
    // Event handler
    int prevTime{};
    int currentTime{};
    int delta = {};
    int delta_x = {};
    int delta_y = {};
    int map_width = {};
    int map_height = {};
    SDL_Rect draw_rect{};
    auto tex = resources_.get_sprite("map");
    SDL_QueryTexture(tex.texture, nullptr, nullptr, &map_width, &map_height);
    SDL_Rect position = {0, 0, map_width, map_height};
    SDL_Rect cameraRect = {0, 0, 1024, 1024};
    // Handle events on queue
    // Get map width and height for the scrolling limits
    SDL_Event ev{};
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    while (running)
    {
        prevTime = currentTime;
        Uint32 currentTime = SDL_GetTicks();
        delta = static_cast<int>(currentTime - prevTime);
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
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (ev.button.button == SDL_BUTTON_LEFT)
                {
                    mouse_button_states_[LEFT] = false;
                }
            /* Look for a keypress */
            case SDL_KEYDOWN:
                if (ev.key.keysym.sym == SDLK_q)
                {
                    // quit
                    running = false;
                }
                if (ev.key.keysym.sym == SDLK_w)
                {
                    // move up
                    LOG(INFO) << "Camera rect y" << cameraRect.y << std::endl;
                    LOG(INFO) << "Position y" << position.y << std::endl;
                    LOG(INFO) << "Map Width:" << map_height;
                    if (position.y > 0)
                    {
                        position.y -= scrollSpeed;
                    }
                    if (position.y < 0)
                    {
                        position.y = 0;
                    }
                }
                if (ev.key.keysym.sym == SDLK_a)
                {
                    // move left;
                    LOG(INFO) << "Camera rect x" << cameraRect.x << std::endl;
                    LOG(INFO) << "Position x" << position.x << std::endl;
                    LOG(INFO) << "Map Width:" << map_width;
                    if (position.x > 0)
                    {
                        position.x -= scrollSpeed;
                    }
                    if (position.x < 0)
                    {
                        position.x = 0;
                    }
                }
                if (ev.key.keysym.sym == SDLK_d)
                {
                    // move right
                    LOG(INFO) << "Map Width:" << map_width;
                    LOG(INFO) << "Camera rect w" << cameraRect.w << std::endl;
                    LOG(INFO) << "Position x" << position.x << std::endl;
                    if (position.x < cameraRect.w)
                    {
                        position.x += scrollSpeed;
                    }
                    if (position.x > cameraRect.w)
                    {
                        position.x = cameraRect.w;
                    }
                }
                if (ev.key.keysym.sym == SDLK_s)
                {
                    // move down
                    LOG(INFO) << "Map Height:" << height;
                    LOG(INFO) << "Camera rect h" << cameraRect.h << std::endl;
                    LOG(INFO) << "Position y" << position.y << std::endl;
                    if (position.y < cameraRect.h)
                    {
                        position.y += scrollSpeed;
                    }
                    if (position.y > cameraRect.h)
                    {
                        position.y = cameraRect.h;
                    }
                }
                if (ev.type == SDL_QUIT)
                {
                    running = false;
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
            }
            }
            draw_rect = {position.x - cameraRect.x, position.y - cameraRect.y, position.w - cameraRect.w, position.h - cameraRect.h};

            // clear screen
            SDL_RenderClear(renderer_);
            SDL_RenderCopy(renderer_, tex.texture, &draw_rect, NULL);
            // Render texture to screen
            // SDL_RenderCopy(renderer_, tex, NULL, &a);
            SDL_RenderPresent(renderer_);
        }
        if (delta < delay_time)

        {
            SDL_Delay((int)(delay_time - delta));
        }
    }
}
