#include "kingdom.h"
#include "common.h"
#include "player.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "astar.h"

using namespace king;

Kingdom::Kingdom()
    : kingdom_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
    , render_(this, &resource_manager_)
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

void Kingdom::setup_kingdom()
{
    // TODO: break this out as a json that comes with the map
    set_astar_data(resource_manager_.get_astar_data());
    start_threads();
    Sprite* world = resource_manager_.get_image("map");
    map_tile_size_x_ = 100;
    map_tile_size_y_ = 100;
    input_.setup_world_data(world->source_rect.w, world->source_rect.h);
    map_height_ = world->source_rect.h;
    map_width_ = world->source_rect.w;
    SDL_GetWindowSize(&render_.get_window(), &window_width_, &window_height_);
    // setup layers
    // launch movement, ai and event threads
    // Calculate any randoms and give armies initial positions in the map
    // Add Army to "movement" queue. Army needs to inherit "behaviour"
    // Setup castles (hardcoded for now)
    Position castle1_pos{ 150, 305 };
    Position castle2_pos{ 1485, 695 };
    Position castle3_pos{ 213, 1263 };
    Position castle4_pos{ 1806, 1737 };
    auto castle1 = std::make_unique<Castle>(Castle(castle1_pos));
    auto castle2 = std::make_unique<Castle>(Castle(castle2_pos));
    auto castle3 = std::make_unique<Castle>(Castle(castle3_pos));
    auto castle4 = std::make_unique<Castle>(Castle(castle4_pos));
    ids_.push_back(castle1->get_id());
    ids_.push_back(castle2->get_id());
    ids_.push_back(castle3->get_id());
    ids_.push_back(castle4->get_id());
    castles_[castle1->get_id()] = std::move(castle1);
    castles_[castle2->get_id()] = std::move(castle2);
    castles_[castle3->get_id()] = std::move(castle3);
    castles_[castle4->get_id()] = std::move(castle4);

    // randomize a start castle
    const auto rnd = common::get_random_value_within_range(0, 3);
    player_.set_castle(ids_[rnd]);
    castles_[ids_[rnd]]->set_owner(player_.get_id());
    auto pos = castles_[ids_[rnd]]->get_position();
    player_.set_position(pos);

    // assign army to player and start it at the same position
    LOG(INFO) << "X: " << player_.get_position().x << " Y: " << player_.get_position().y;
    auto army = std::make_unique<Army>(
        Army(player_, player_.get_position(), resource_manager_.get_image("human-swordman"), 100));
    auto id = army->get_id();
    armies_[id] = std::move(army);
}

void Kingdom::start_threads()
{
    g_running = true;
    post(pool, [this]() { this->movement(); });
    post(pool, [this]() { this->ai(); });
    post(pool, [this]() { this->weather(); });
    post(pool, [this]() { this->event(); });
    post(pool, [this]() { this->process(); });
}

void Kingdom::stop_threads()
{
    LOG(INFO) << "stopping threads...";
    pool.join();
}

const std::vector<int>& Kingdom::get_astar_result() const
{
    return astar_.get_final_path();
}

int Kingdom::get_tile_sizes()
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

void Kingdom::draw_sprites(SDL_Renderer* renderer) const
{
    for (auto const& val : armies_)
    {
        val.second->draw(renderer);
    }
}

const KingdomID& Kingdom::get_id() const
{
    return kingdom_id_;
}

void Kingdom::movement()
{
    LOG(INFO) << "staring movement thread..";
    while (g_running)
    {
        LOG(INFO) << "movement thread running executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        /*
         * Armies are player controlled and ai controlled.
         * A player army can also become ai controlled if they feel that the player
         * is not taking care of them.
         * TODO: Create a move from a to b using a*, plot it on the map using a spline/bezier line.
         * This would mean, plot the path with a* taking into accounts the weights of the map.
         * Figure out where the control points go.
         */
         // Iterate through all the army structures and call the move method
        for (auto const& [owner, army] : armies_)
        {
            // if army id is owned by player then move to destination set by player, not by ai
            if (owner == player_.get_id())
            {
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
    LOG(INFO) << "starting event thread...";
    while (g_running)
    {
        LOG(INFO) << "event thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Kingdom::process()
{
    LOG(INFO) << "starting process thread...";
    while (g_running)
    {
        // process mouse selections
        if (input_.get_left_mouse_button_state())
        {
            for (auto const& [id, army] : armies_)
            {
                const auto mouse_coords = input_.left_mouse_button_entry();
                if (coords_within_square(mouse_coords.x_pos, mouse_coords.y_pos, army.get()->get_sprite_rect()))
                {
                    // Draw a path with astar from one castle to another.
                    //
                    // Take the clicked army and move it to another castle
                    // this means fill the astar with the world weight data
                    // grab another castle (for now) and make astar trace a path to the castle
                    // move the unit through the path to the other castle.
                    // win
                    //

                    const auto rect = army.get()->get_sprite_rect();
                    const auto [start_x, start_y] = get_square(Position{rect.x, rect.y});

                    auto castle_id = get_castle_id(3);
                    const Castle& dest = get_castle(castle_id);
                    const auto pos = dest.get_position();
                    const auto [dest_x, dest_y] = get_square(pos);
                    MovementPath& path = army.get()->get_movement_path();
                    path.resize(map_tile_size_x_ * map_tile_size_y_);
                    astar_.astar(start_x, start_y, dest_x, dest_y, false);
                    int i = 0;
                }
            }
        }
    }
}

Position Kingdom::get_square(const Position& pos) const
{
    const auto tile_width = map_width_ / map_tile_size_x_;
    const auto tile_height = map_height_ / map_tile_size_y_;
    return Position{ pos.x / tile_width, pos.y / tile_height};
}

bool Kingdom::coords_within_square(int x, int y, const SDL_Rect& rect) const
{
    const float scale_x = input_.get_scale() * static_cast<float>(window_width_) / static_cast<float>(map_width_);
    const float scale_y = input_.get_scale() * static_cast<float>(window_height_) / static_cast<float>(map_height_);
    x = static_cast<int>(static_cast<float>(x) / scale_x);
    y = static_cast<int>(static_cast<float>(y) / scale_y);

    if ((x > rect.x) && (x < rect.x + rect.w) && (y > rect.y) && (y < rect.y + rect.h))
    {
        return true;
    }
    return false;
}

CastleID& Kingdom::get_castle_id(const int position)
{
    return ids_[position];
}
