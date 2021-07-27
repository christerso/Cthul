// Chtuhl.cpp : Defines the entry point for the application.
//

#include "Cthul.h"

#include <fmt/format.h>

using namespace RM;

Cthul::Cthul() {}

Cthul::~Cthul()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
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
    m_window = nullptr;
    SDL_Quit();

    LOG(INFO) << "SDL2 is exiting..." << std::endl;
}

void Cthul::create_window()
{

    // Create window
    m_window = SDL_CreateWindow("Cthul", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    if (m_window == nullptr)
    {
        LOG(ERROR) << fmt::format("Window could not be created !SDL_Error: {}", SDL_GetError());
        shutdown_SDL2();
    }
    else
    {
        // Get window surface
        m_screen_surface = SDL_GetWindowSurface(m_window);
    }
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr)
    {
        LOG(ERROR) << fmt::format("renderer could not be created! SDL Error: {}", SDL_GetError());
    }
    m_resources.set_renderer(m_renderer);
    // initialize renderer color
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void Cthul::setup_resources()
{
    m_resources.setup_initial_resources();
}

void Cthul::start_input_loop()
{
    bool running = true;
    bool quit = false;

    // Event handler
    SDL_Event e;

    int delta_x = 0;
    int delta_y = 0;
    SDL_Rect a{0,0,100,100};
    auto tex = m_resources.get_image("Adam");
    // Handle events on queue
    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_TEXTINPUT)
            {

                LOG(INFO) << e.text.text << std::endl;
            }
            else if (e.type = SDL_MOUSEMOTION)
            {
                delta_x += e.motion.x;
                delta_y += e.motion.y;
            }
        }
        // clear screen
        SDL_RenderClear(m_renderer);

        // Render texture to screen
        SDL_RenderCopy(m_renderer, tex, NULL, &a);
        SDL_RenderPresent(m_renderer);
    }
}
