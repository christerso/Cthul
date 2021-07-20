#include "SDL2Wrapper.h"
#include "ResourceManager.h"
#include <fmt/format.h>

using namespace SW;

SDL_Window* g_window = nullptr;
SDL_Surface* g_screen_surface = nullptr;

void SDL2Wrapper::initialize_SDL2()
{
    if ((SDL_Init(SDL_INIT_EVERYTHING) == -1))
    {
        LOG(ERROR) << "Could not initialize SDL: {}", SDL_GetError();
        exit(-1);
    }
}

void SDL2Wrapper::shutdown_SDL2()
{
    /* Shutdown all subsystems */
    g_window = nullptr;
    SDL_Quit();

    LOG(INFO) << "SDL2 is exiting..." << std::endl;
}

void SDL2Wrapper::create_window()
{
    // Create window
    g_window = SDL_CreateWindow("Chthul", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    if (g_window == nullptr)
    {
        LOG(ERROR) << fmt::format("Window could not be created !SDL_Error: {}\n", SDL_GetError());
        SDL2Wrapper::shutdown_SDL2();
    }
    else
    {
        // Get window surface
        g_screen_surface = SDL_GetWindowSurface(g_window);
    }

        // TMP TEST CODE:
    RM::ResourceManager r;
    auto surface = r.load("Tst", "test-images/cthulhu.png");
    SDL_Renderer * renderer = SDL_CreateRenderer(g_window, -1, 0);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
/*
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(g_window);
*/
}

void SDL2Wrapper::start_input_loop()
{
    bool running = true;
    bool quit = false;

    // Event handler
    SDL_Event e;

    int delta_x = 0;
    int delta_y = 0;

    // Handle events on queue
    while (running)
    {
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
                DLOG(INFO) << "X:" << e.motion.x << " Y:" << e.motion.y;
                delta_x += e.motion.x;
                delta_y += e.motion.y;
            }
        }
    }

    SDL_UpdateWindowSurface(g_window);
}
