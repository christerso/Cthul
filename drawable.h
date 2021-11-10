#pragma once
#include <SDL2/SDL.h>

namespace king
{
class Drawable
{
public:
    virtual void draw(SDL_Renderer* renderer) = 0;

    static void scale_object(SDL_Point& center, const SDL_Rect& origin, SDL_Rect& new_position, const int scale)
    {
        center = {origin.w / 2, origin.h / 2};
        new_position = {origin.x, origin.y, origin.w / scale, origin.h / scale};
        new_position.x = new_position.x - new_position.w / 2;
        new_position.y = new_position.y - new_position.h;
    }
};

} // namespace king