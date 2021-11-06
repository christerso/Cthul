#include "kingdom.h"
#include "player.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace king;

Kingdom::Kingdom()
    : kingdom_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
{
}

Kingdom::~Kingdom()
{
    stop_threads();
}

std::vector<Castle*> Kingdom::get_castles() const
{
    std::vector<Castle*> castles;
    for (auto const& [key, val] : castles_)
    {
        castles.push_back(val.get());
    }
    return castles;
}

std::map<ArmyID, std::unique_ptr<Army>>& Kingdom::get_armies()
{
    return armies_;
}

void Kingdom::setup_kingdom()
{
    resource_manager_ = std::make_unique<ResourceManager>(ResourceManager());
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
    castles_[castle1->get_id()] = std::move(castle1);
    castles_[castle2->get_id()] = std::move(castle2);
    castles_[castle3->get_id()] = std::move(castle3);
    castles_[castle4->get_id()] = std::move(castle4);

    // assign castle to player
    std::vector<Castle*> castles = get_castles();
    for (auto const& castle : castles)
    {
        if (castle->get_owner().empty())
        {
            player_.set_castle(castle);
            castle->set_owner(player_.get_id());
            player_.set_position(castle->get_position());
            break;
        }
    }
    // assign army to player and start it at the same position
    auto army = std::make_unique<Army>(Army(player_, player_.get_position(), resource_manager_->get_sprite("human-swordman"), 100));
    auto id = army->get_id();
    armies_[id] = std::move(army);
}

void Kingdom::start_threads()
{
    bool run = true;
    post(pool_, [this]() { this->movement(); });
    post(pool_, [this]() { this->ai(); });
    post(pool_, [this]() { this->weather(); });
    post(pool_, [this]() { this->event(); });
}

void Kingdom::stop_threads()
{
    LOG(INFO) << "stopping threads...";
    pool_.join();
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

void Kingdom::draw_sprites(SDL_Renderer* renderer)
{
    SDL_FRect destination {};
    LOG(INFO) << "Armies: " << armies_.size();
    for (auto const& [key, val] :armies_)
    {
        val->draw(renderer);
    }
}

void Kingdom::set_input_handler(Input* input)
{
    input_ = input;
}

const KingdomID& Kingdom::get_id() const
{
    return kingdom_id_;
}

void Kingdom::movement()
{
    LOG(INFO) << "staring movement thread..";
    while (true)
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
    while (true)
    {
        LOG(INFO) << "ai thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Kingdom::weather()
{
    LOG(INFO) << "starting weather thread...";
    while (true)
    {
        LOG(INFO) << "weather thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }
}

void Kingdom::event()
{
    LOG(INFO) << "starting event thread...";
    while (true)
    {
        LOG(INFO) << "event thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Kingdom::process()
{
    LOG(INFO) << "starting process thread...";
    while (true)
    {
        LOG(INFO) << "process thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // process mouse selections
        for (auto const& [id, army] : armies_)
        {
        }
    }
}
