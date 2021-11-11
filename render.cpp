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

    if (SDL_CreateWindowAndRenderer(1200, 1200, SDL_WINDOW_BORDERLESS, &window_, &renderer_))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    screen_surface_ = SDL_GetWindowSurface(window_);
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
    camera_ = {0, 0, map_width_ / 2, map_height_ / 2};
    input_->set_camera(camera_);
}

SDL_Rect& Render::get_camera()
{
    return camera_;
}

void Render::draw_world()
{
    camera_ = input_->get_camera();
    scale_ = input_->get_scale();
    previous_time_ = current_time_;
    current_time_ = SDL_GetTicks();
    delta_ = current_time_ - previous_time_;
    updated_delta_ += delta_;
    delay_time_ = static_cast<int>(1000.0f / fps_);

    SDL_SetRenderTarget(renderer_, texture_buffer_);
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, map_texture_->texture, nullptr, nullptr);
    kingdom_->draw_sprites(renderer_);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderCopy(renderer_, texture_buffer_, &camera_, nullptr);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderPresent(renderer_); // swap in the final image

    if (delta_ < delay_time_)
    {
        SDL_Delay(delay_time_ - delta_);
    }
}
