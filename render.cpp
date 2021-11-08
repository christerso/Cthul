#include "render.h"
#include "kingdom.h"
#include <fmt/format.h>
#include <glog/logging.h>

using namespace king;

Render::Render(Kingdom* kingdom, ResourceManager* resource_manager)
    : kingdom_(kingdom)
    , resource_manager_(resource_manager)
{
    setup();
}

Render::~Render()
{
    SDL_DestroyTexture(texture_buffer_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    LOG(INFO) << "Render destructor called";
}

bool Render::create_window()
{
    // Create window
    window_ = SDL_CreateWindow("Kings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1200, SDL_WINDOW_OPENGL);
    if (window_ == nullptr)
    {
        LOG(ERROR) << fmt::format("Window could not be created !SDL_Error: {}", SDL_GetError());
        return false;
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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    resource_manager_->set_renderer(renderer_);
    // initialize renderer color
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    return true;
}

void Render::setup()
{
    input_ = &kingdom_->get_input();
    create_window();
    resource_manager_->setup_initial_resources();
    // main world map saved in a sprite structure inside resource manager
    map_texture_ = resource_manager_->get_image("map");
    // set up offscreen image to copy things to
    SDL_QueryTexture(map_texture_->texture, nullptr, nullptr, &map_width_, &map_height_);

    // render to this texture before displaying
    texture_buffer_ =
        SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, map_width_, map_height_);
    if (texture_buffer_ == nullptr)
    {
        LOG(ERROR) << fmt::format("renderer target could not be created! SDL Error: {}", SDL_GetError());
        return;
    }
    position_ = {0, 0, map_width_, map_height_};
    camera_ = {0, 0, map_width_ / 4, map_height_ / 4};

}

SDL_Rect& Render::get_camera()
{
    return camera_;
}

SDL_Rect& Render::get_world_size()
{
    return position_;
}

void Render::draw_world()
{
    position_ = input_->get_position();
    previous_time_ = current_time_;
    current_time_ = SDL_GetTicks();
    delta_ = current_time_ - previous_time_;
    updated_delta_ += delta_;
    delay_time_ = static_cast<int>(1000.0f / fps_);
    draw_area_ = {position_.x, position_.y, position_.w - camera_.w, position_.h - camera_.h};
    SDL_SetRenderTarget(renderer_, texture_buffer_);
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, map_texture_->texture, nullptr, nullptr);
    kingdom_->draw_sprites(position_, renderer_);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderCopy(renderer_, texture_buffer_, &draw_area_, nullptr);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderPresent(renderer_); // swap in the final image

    if (delta_ < delay_time_)
    {
        SDL_Delay(delay_time_ - delta_);
    }
}
