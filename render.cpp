#include "kingdom.h"
#include "render.h"
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

void Render::draw_line(float start_x, float start_y, float end_x, float end_y) const
{
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLineF(renderer_, start_x, start_y, end_x, end_y);
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

void Render::draw_world()
{
    camera_ = input_->get_camera();
    scale_ = input_->get_scale();
    delay_time_ = static_cast<int>(1000.0f / fps_);

    SDL_SetRenderTarget(renderer_, texture_buffer_);
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, map_texture_->texture, nullptr, nullptr);
    kingdom_->draw_sprites(renderer_);
    kingdom_->draw_bezier_paths(renderer_);
    if (input_->show_astar_path()) {
        draw_debug_astar();
        draw_all_astar_weights();
    }

    SDL_RenderDrawRect(renderer_, &input_->highlight_square_);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderCopy(renderer_, texture_buffer_, &camera_, nullptr);
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderPresent(renderer_); // swap in the final image

    if (Timer::instance()->delta_time() < delay_time_)
    {
        SDL_Delay(delay_time_ - static_cast<int>(Timer::instance()->delta_time()));
    }
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
    SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_ADD);
    for (auto& position : path)
    {
        auto pos_x = position % tile_pos_y;
        auto pos_y = position / tile_pos_x;
        square = { pos_x * tile_size, pos_y * tile_size, tile_size, tile_size };
        SDL_RenderFillRect(renderer_, &square);
    }
}

void Render::draw_all_astar_weights()
{
    auto data = resource_manager_->get_astar_data();
    if (data.empty())
    {
        return;
    }
    SDL_Rect square{};
    auto tile_pos_x = 100;
    auto tile_pos_y = 100;
    auto tile_size = map_width_ / 100;
    std::vector<int> colors;
    int base_color = 0;
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    for (size_t j = 0; j < 8; j++)
    {
        colors.push_back(base_color);
        base_color += (255 / 8);
    }
    for (auto i = 0; i < data.size(); i++)
    {
        auto pos_x = i % tile_pos_y;
        auto pos_y = i / tile_pos_x;

        square = { pos_x * tile_size, pos_y * tile_size, tile_size, tile_size };

        SDL_SetRenderDrawColor(renderer_, colors[data[i]], colors[data[i]], colors[data[i]], 155);
        SDL_RenderFillRect(renderer_, &square);
    }

}
