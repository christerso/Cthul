#include "kingdom.h"
#include "common.h"
#include "player.h"
#include <glm/vec2.hpp>
#include <chrono>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "astar.h"
#include "path.h"
#include "fastnoiselite.h"
#include <immintrin.h>

using namespace king;

Kingdom::Kingdom()
    : kingdom_id_(to_string(boost::uuids::random_generator()()))
    , render_(this, &resource_manager_)
    , timer_(*Timer::instance())
{
}

Kingdom::~Kingdom()
{
    LOG(INFO) << "Kingdom destructor called";
}

void Kingdom::run()
{
    setup_kingdom();

    while (input_.input_loop())
    {
        timer_.update();
        render_.draw_world();
    }
    // stop threads
    g_running = false;
    stop_threads();
}

const Castle& Kingdom::get_castle(const CastleID& id) const
{
    return *castles_.at(id).get();
}

std::map<ArmyID, std::unique_ptr<Army>>& Kingdom::get_armies()
{
    return armies_;
}

// The kingdom size is 120km by 120km
// This mean that each astar grid is 1.2km.
// The speed to traverse an astar grid of 1.2km are (if 5km/h)

void Kingdom::setup_kingdom()
{
    // TODO: break this out as a json that comes with the map
    set_astar_data(resource_manager_.get_astar_data());
    start_threads();
    Sprite* world = resource_manager_.get_image("map");
    input_.setup_world_data(world->source_rect.w, world->source_rect.h);
    map_height_ = world->source_rect.h;
    map_width_ = world->source_rect.w;
    map_tile_size_x_ = map_height_ / 100;
    map_tile_size_y_ = map_width_ / 100;
    SDL_GetWindowSize(&render_.get_window(), &window_width_, &window_height_);
    // setup layers
    // launch movement, ai and event threads
    // Calculate any randoms and give armies initial positions in the map
    // Add Army to "movement" queue. Army needs to inherit "behaviour"
    // Setup castles (hardcoded for now)
    glm::vec2 castle1_pos{ 150, 305 };
    glm::vec2 castle2_pos{ 1485, 695 };
    glm::vec2 castle3_pos{ 213, 1263 };
    glm::vec2 castle4_pos{ 1806, 1737 };
    auto castle1 = std::move(std::make_unique<Castle>(Castle(castle1_pos)));
    auto castle2 = std::move(std::make_unique<Castle>(Castle(castle2_pos)));
    auto castle3 = std::move(std::make_unique<Castle>(Castle(castle3_pos)));
    auto castle4 = std::move(std::make_unique<Castle>(Castle(castle4_pos)));
    ids_.push_back(castle1->get_id());
    ids_.push_back(castle2->get_id());
    ids_.push_back(castle3->get_id());
    ids_.push_back(castle4->get_id());
    castles_[castle1->get_id()] = std::move(castle1);
    castles_[castle2->get_id()] = std::move(castle2);
    castles_[castle3->get_id()] = std::move(castle3);
    castles_[castle4->get_id()] = std::move(castle4);

    // randomize a start castle
    const auto rnd = common::RangeRand<int>::get_random_value_within_range(0, 3);
    player_.set_castle(ids_[rnd]);
    castles_[ids_[rnd]]->set_owner(player_.get_id());
    auto pos = castles_[ids_[rnd]]->get_position();
    player_.set_position(pos);

    // assign army to player and start it at the same position
    LOG(INFO) << "X: " << player_.get_position().x << " Y: " << player_.get_position().y;
    auto& nonconstpos = player_.get_position();
    auto army = std::make_unique<Army>(Army(player_, nonconstpos, resource_manager_.get_image("human-swordman"), 100));
    auto army1 = std::make_unique<Army>(Army(player_, castle4_pos, resource_manager_.get_image("human-axe-rider"), 100));
    auto id = army->get_id();
    armies_[id] = std::move(army);
    auto id1 = army1->get_id();
    armies_[id1] = std::move(army1);
}

void Kingdom::start_threads()
{
    g_running = true;
    post(pool, [this]()->void { this->movement(); });
    post(pool, [this]()->void { this->ai(); });
    post(pool, [this]()->void { this->weather(); });
    post(pool, [this]()->void { this->event(); });
    post(pool, [this]()->void { this->process(); });
}

void Kingdom::stop_threads()
{
    LOG(INFO) << "stopping threads...";
    input_.process_cv.notify_one();
    pool.join();
}

std::vector<int>& Kingdom::get_astar_result_limited(int step)
{
    return astar_.get_final_path_limited(step);
}

std::vector<int>& Kingdom::get_astar_result()
{
    return astar_.get_final_path();
}

int Kingdom::get_tile_sizes() const
{
    return map_tile_size_x_ / 100;
}

void Kingdom::set_astar_data(std::vector<int>& data)
{
    astar_.bind(data.data(), 100, 100);
}

void Kingdom::spawn_new_army()
{
    // Get position of castle and spawn default army
    // The castle id should be part of player in the future.
}

Input& Kingdom::get_input()
{
    return input_;
}

void Kingdom::draw_paths(SDL_Renderer* renderer) const
{
    for (const auto& army : armies_)
    {

        // note this is a copy by value
        auto army_movement_path = army.second->movement_path.get_path();
        auto it_p = army_movement_path.begin();

        glm::vec2 from{}, to{};
        while (it_p != army_movement_path.end())
        {
            from = { it_p->x, it_p->y };
            ++it_p;
            if (it_p != army_movement_path.end())
            {
                to = { it_p->x, it_p->y };
            }
            else
            {
                to = { from.x, from.y };
            }
            SDL_RenderDrawLineF(renderer, from.x, from.y, to.x, to.y);
        }
    }
}

void Kingdom::draw_sprites(SDL_Renderer* renderer) const
{
    for (const auto& val : armies_)
    {
        val.second->draw(renderer);
    }
}

const KingdomID& Kingdom::get_id() const
{
    return kingdom_id_;
}

/*
 * Iterates through all the movables, calles the update method.
 * The update method will check if there exists a curve, if a curve is found, move along the curve.
 */


void Kingdom::movement()
{
    while (g_running)
    {
        for (const auto& [owner, army] : armies_)
        {
            if (army.get() == nullptr)
            {
                continue;
            }
            if (army->path_active)
            {
                army->move(Origin::kPlayer);
            }
        }
    }
}

void Kingdom::ai()
{
    LOG(INFO) << "starting ai thread...";
    while (g_running)
    {
        LOG(INFO) << "ai thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Kingdom::weather()
{
    LOG(INFO) << "starting weather thread...";
    while (g_running)
    {
        LOG(INFO) << "weather thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }
}

void Kingdom::event()
{
    std::mutex event_lock;
    std::unique_lock lk(event_lock);
    LOG(INFO) << "starting event thread...";
    while (g_running)
    {
        LOG(INFO) << "event thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// Kingdom::process() is responsible for
// 1. astar calculations
// 2. acting on mouse input
constexpr int kCurveSamples = 10;
void Kingdom::process()
{
    std::mutex process_lock;
    LOG(INFO) << "starting process thread...";
    std::unique_lock lk(process_lock);
    while (g_running)
    {
        input_.process_cv.wait(lk);
        MouseQueueEvent event;
        input_.get_next_mouse_action(event);

        LOG(INFO) << "processing thread executing...";
        if (event.action == kNone)
        {
            continue;
        }
        if (event.action == kLeft)
        {
            for (const auto& [id, army] : armies_)
            {
                if (coords_within_square(event.position.x_pos, event.position.y_pos, army->get_sprite_rect()))
                {
                    input_.add_selected_army(id);
                }
            }
        }
        if (event.action == kRight)
        {
            const SelectedArmies& armies = input_.get_selected_armies();
            if (armies.empty())
            {
                continue;
            }
            for (auto& army_id : armies)
            {
                // for each army selected, call astar and plot a path, storing the path in the army itself.
                // this plotting should be postponed in the full implementation, taking message delivery time to the army 
                glm::vec2 mouse_pos;
                Army* army = armies_[army_id].get();
                army->movement_path.clear_path();
                const glm::vec2 tile_position = common::get_square(map_tile_size_x_, map_tile_size_y_, army->get_position());
                mouse_to_screen_coords(event.position.x_pos, event.position.y_pos, mouse_pos);
                const auto destination = common::get_square(map_tile_size_x_, map_tile_size_y_, mouse_pos);
                if (astar_.astar(static_cast<int>(tile_position.x), static_cast<int>(tile_position.y), static_cast<int>(destination.x), static_cast<int>(destination.y), false))
                {
                    auto& result = get_astar_result_limited(3);
                    //army->path_distance = calculate_path_length(result);

                    while (result.size() % 4 != 0)
                    {
                        result.push_back(result[result.size() - 1]);
                    }

                    auto it_pos = result.begin();
                    auto it_end = result.end();
                    while (it_pos != it_end)
                    {
                        Curve curve{};
                        curve.p0 = get_square_pixel_position(*it_pos++);
                        curve.p1 = get_square_pixel_position(*it_pos++);
                        if (it_pos != it_end)
                        {
                            curve.p2 = get_square_pixel_position(*it_pos++);
                        }
                        else
                        {
                            curve.p2 = curve.p1;
                            curve.p3 = curve.p1;
                            army->movement_path.add_curve(curve, kCurveSamples);
                            continue;
                        }
                        if (it_pos != it_end)
                        {
                            curve.p3 = get_square_pixel_position(*it_pos);
                        }
                        else
                        {
                            curve.p3 = curve.p2;
                            army->movement_path.add_curve(curve, kCurveSamples);
                            continue;
                        }
                        --it_pos;
                        --it_pos;
                        army->movement_path.add_curve(curve, kCurveSamples);
                    }
                    army->init_path();
                }
            }
            input_.clear_selected_armies();
        }
    }
}

glm::vec2 Kingdom::get_square_pixel_position(int position) const
{
    constexpr int tile_pos_y = 100;
    constexpr int tile_pos_x = 100;
    const auto tile_size = map_width_ / 100;
    const auto pos_x = position % tile_pos_y;
    const auto pos_y = position / tile_pos_x;
    return glm::vec2{ pos_x * tile_size + (tile_size / 2), pos_y * tile_size + (tile_size / 2) };
}

void Kingdom::mouse_to_screen_coords(int x, int y, glm::vec2& position) const
{
    const float scale_x = input_.get_scale() * static_cast<float>(window_width_) / static_cast<float>(map_width_);
    const float scale_y = input_.get_scale() * static_cast<float>(window_height_) / static_cast<float>(map_height_);
    position.x = x / scale_x;
    position.y = y / scale_y;
}

bool Kingdom::coords_within_square(int x, int y, const SDL_Rect& rect) const
{
    glm::vec2 pos;
    mouse_to_screen_coords(x, y, pos);

    if ((pos.x > rect.x) && (pos.x < rect.x + rect.w) && (pos.y > rect.y) && (pos.y < rect.y + rect.h))
    {
        return true;
    }
    return false;
}

CastleID& Kingdom::get_castle_id(const int position)
{
    return ids_[position];
}
