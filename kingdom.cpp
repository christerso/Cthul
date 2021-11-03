#include "kingdom.h"

#include "common.h"
#include "player.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>

using namespace king;

Kingdom::Kingdom(ThreadManager& thread_manager)
    : kingdom_id_(boost::uuids::to_string(boost::uuids::random_generator()()))
    , thread_manager_(thread_manager)
{
    setup_kingdom();
}

Kingdom::~Kingdom() {}

std::vector<Castle*> Kingdom::get_castles() const
{
    std::vector<Castle*> castles;
    for (auto const& [key, val] : castles_)
    {
        castles.push_back(val.get());
    }
    return castles;
}

void Kingdom::setup_kingdom()
{
    // launch movement, ai and event threads

    // Calculate any randoms and give armies initial positions in the map
    // Add Army to "movement" queue. Army needs to inherit "behaviour"
    // Setup castles (hardcoded for now)
    Position castle1_pos{150, 305};
    Position castle2_pos{1485, 695};
    Position castle3_pos{213, 1263};
    Position castle4_pos{1806, 1737};
    auto castle1 = std::make_unique<Castle>(Castle(castle2_pos));
    auto castle2 = std::make_unique<Castle>(Castle(castle3_pos));
    auto castle3 = std::make_unique<Castle>(Castle(castle1_pos));
    auto castle4 = std::make_unique<Castle>(Castle(castle4_pos));
    castles_[castle1->get_id()] = std::move(castle1);
    castles_[castle2->get_id()] = std::move(castle2);
    castles_[castle3->get_id()] = std::move(castle3);
    castles_[castle4->get_id()] = std::move(castle4);

    // assign castle to player
    std::vector<Castle*> castles = get_castles();
    for (auto const& entry : castles)
    {
        if (entry->get_owner().empty())
        {
            player_ = std::make_unique<Player>(Player(entry));
            break;
        }
    }

    ThreadMethod f = [this]() { this->movement(); };
    thread_manager_.pool.push_task(f);
    f = [this]() { this->ai(); };
    thread_manager_.pool.push_task(f);
    f = [this]() { this->weather(); };
    thread_manager_.pool.push_task(f);
    f = [this]() { this->event(); };
    thread_manager_.pool.push_task(f);
}

void Kingdom::spawn_new_army()
{
    // Get position of castle and spawn default army
    // The castle id should be part of player in the future.
}

const KingdomID& Kingdom::get_id() const
{
    return kingdom_id_;
}

void Kingdom::movement()
{
    LOG(INFO) << "staring movement thread..";
    while (!thread_manager_.quit)
    {
        LOG(INFO) << "movement thread running executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Kingdom::ai()
{
    LOG(INFO) << "starting ai thread...";
    while (!thread_manager_.quit)
    {
        LOG(INFO) << "ai thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Kingdom::weather()
{
    LOG(INFO) << "starting weather thread...";
    while (!thread_manager_.quit)
    {
        LOG(INFO) << "weather thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }
}

void Kingdom::event()
{
    LOG(INFO) << "starting event thread...";
    while (!thread_manager_.quit)
    {
        LOG(INFO) << "event thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
