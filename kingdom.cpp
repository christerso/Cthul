#include "kingdom.h"
#include "common.h"
#include "player.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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

std::vector<Castle*> Kingdom::get_castles() const
{
    std::vector<Castle*> castles;
    for (auto& val : castles)
    {
        castles.push_back(val);
    }
    return castles;
}

std::map<ArmyID, std::unique_ptr<Army>>& Kingdom::get_armies()
{
    return armies_;
}

void Kingdom::setup_kingdom()
{
    set_astar_data(resource_manager_.get_astar_data());
    start_threads();
    Sprite* world = resource_manager_.get_image("map");
    input_.setup_world_data(world->source_rect.w, world->source_rect.h);
    // setup layers
    // launch movement, ai and event threads
    // Calculate any randoms and give armies initial positions in the map
    // Add Army to "movement" queue. Army needs to inherit "behaviour"
    // Setup castles (hardcoded for now)
    Position castle1_pos{150, 305};
    Position castle2_pos{1485, 695};
    Position castle3_pos{213, 1263};
    Position castle4_pos{1806, 1737};
    auto castle1 = std::make_unique<Castle>(Castle(castle1_pos));
    auto castle2 = std::make_unique<Castle>(Castle(castle2_pos));
    auto castle3 = std::make_unique<Castle>(Castle(castle3_pos));
    auto castle4 = std::make_unique<Castle>(Castle(castle4_pos));
    std::vector<CastleID> ids;
    ids.push_back(castle1->get_id());
    ids.push_back(castle2->get_id());
    ids.push_back(castle3->get_id());
    ids.push_back(castle4->get_id());
    castles_[castle1->get_id()] = std::move(castle1);
    castles_[castle2->get_id()] = std::move(castle2);
    castles_[castle3->get_id()] = std::move(castle3);
    castles_[castle4->get_id()] = std::move(castle4);

    // randomize a start castle
    const auto rnd = common::get_random_value_within_range(0, 3);
    player_.set_castle(ids[rnd]);
    castles_[ids[rnd]]->set_owner(player_.get_id());
    player_.set_position(castles_[ids[rnd]]->get_position());

    // assign army to player and start it at the same position
    LOG(INFO) << "X: " << player_.get_position().pos_x << " Y: " << player_.get_position().pos_y;
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

void Kingdom::draw_sprites(SDL_Renderer* renderer)
{
    for (auto const& val : armies_)
    {
        val.second.get()->draw(renderer);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        // process mouse selections
        if (input_.get_left_mouse_button_state())
        {
            for (auto const& [id, army] : armies_)
            {
                auto mouse_coords = input_.left_mouse_button_entry();
                if (coords_within_square(mouse_coords.x_pos, mouse_coords.y_pos, army.get()->get_sprite_rect()))
                {
                    int i = 0;
                }
            }
        }
    }
}

bool Kingdom::coords_within_square(int x, int y, SDL_Rect& rect)
{
    if (x >= rect.x && x<= rect.x + rect.w)
        if (y >= rect.y && y <= rect.y + rect.h)
        {
            return true;
        }
    return false;
}