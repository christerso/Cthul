#pragma once
#include <SDL2/SDL.h>

namespace king
{
class Drawable
{
public:
    virtual void draw(SDL_Rect& position, SDL_Renderer* renderer) = 0;
};
} // namespace kings