#pragma once

#include "input.h"
#include "resourcemanager.h"

#include <SDL2/SDL_render.h>

namespace king
{
class ResourceManager;
class Kingdom;
class Render
{
public:
    Render(Kingdom* kingdom, ResourceManager* resource_manager);
    ~Render();

    void setup();
    SDL_Window& get_window() const;
    SDL_Rect& get_camera();
    bool create_window();
    void draw_debug_astar() const;
    void draw_weights() const;
    void draw_world();
    
private:
    float scale_ {1.0f};
    Uint32 previous_time_{};
    Uint32 current_time_{};
    Uint32 delta_{};
    Uint32 updated_delta_{};
    int fps_{144};
    Uint32 delay_time_{};
    int map_width_{};
    int map_height_{};
    SDL_Rect camera_{0, 0, 0, 0};
    SDL_Renderer* renderer_ = nullptr;
    SDL_Window* window_ = nullptr;
    Sprite* map_texture_;
    SDL_Surface* screen_surface_ = nullptr;
    SDL_Texture* texture_buffer_ = nullptr;
    Kingdom* kingdom_;
    ResourceManager* resource_manager_;
    Input* input_;
};

} // namespace king