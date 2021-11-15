#include "render.h"
#include "kingdom.h"
#include <fmt/format.h>
#include <glog/logging.h>
#include <vectorclass/vectorclass.h>

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
    camera_ = { 0, 0, map_width_, map_height_ };
    input_->set_camera(camera_);
}

SDL_Window& Render::get_window() const
{
    return *window_;
}

SDL_Rect& Render::get_camera()
{
    return camera_;
}

void Render::draw_debug_astar() const
{
    int x{};
    int y{};
    int position{};

    const std::vector<int>& path = kingdom_->get_astar_result();
    if (path.empty())
    {
        return;
    }
    SDL_Rect square{};
    auto tile_pos_x = 100;
    auto tile_pos_y = 100;
    auto tile_size = map_width_ / 100;
    for (auto& position : path)
    {
        auto pos_x = position % tile_pos_y;
        auto pos_y = position / tile_pos_x;
        square = { pos_x * tile_size, pos_y * tile_size, tile_size, tile_size };
        SDL_RenderDrawRect(renderer_, &square);
    }


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

    //if (input_->show_astar_path()) {
        draw_debug_astar();
    //}

    SDL_RenderDrawRect(renderer_, &input_->highlight_square_);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderCopy(renderer_, texture_buffer_, &camera_, nullptr);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderPresent(renderer_); // swap in the final image

    if (delta_ < delay_time_)
    {
        SDL_Delay(delay_time_ - delta_);
    }
}

void Render::draw_weights() const
{
    //std::vector<char> map_ascii;

    //map_ascii.resize(m_map_size_x * m_map_size_y);

    //// Start
    //for (int i = 0; i < m_map_size_x * m_map_size_y; i++)
    //{
    //    switch (m_current_bound[i])
    //    {
    //    case 0:
    //    {
    //        map_ascii[i] = '#';
    //        continue;
    //    }
    //    default:
    //        map_ascii[i] = '-';
    //        continue;
    //    }
    //}
}