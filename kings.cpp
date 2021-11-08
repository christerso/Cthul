// Chtuhl.cpp : Defines the entry point for the application.
//

#include "kings.h"
#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <thread_pool.hpp>

using namespace king;

Kings::Kings()
{
}

Kings::~Kings()
{
    LOG(INFO) << "Kings destructor called";
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

void Kings::start()
{
    initialize_sdl2();
    kingdom_.run();
    shutdown_sdl2();
}

void Kings::shutdown_sdl2()
{
    IMG_Quit();
    SDL_Quit();

    LOG(INFO) << "SDL2 is exiting..." << std::endl;
}

Kingdom& Kings::get_kingdom()
{
    return kingdom_;
}

