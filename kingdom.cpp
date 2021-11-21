#include "kingdom.h"
#include "common.h"
#include "player.h"
#include <glm/vec2.hpp>
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
    common::Position castle1_pos{ 150, 305 };
    common::Position castle2_pos{ 1485, 695 };
    common::Position castle3_pos{ 213, 1263 };
    common::Position castle4_pos{ 1806, 1737 };
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
    const auto rnd = common::RangeRand<int>::get_random_value_within_range(0, 3);
    player_.set_castle(ids_[rnd]);
    castles_[ids_[rnd]]->set_owner(player_.get_id());
    auto pos = castles_[ids_[rnd]]->get_position();
    player_.set_position(pos);

    // assign army to player and start it at the same position
    LOG(INFO) << "X: " << player_.get_position().x << " Y: " << player_.get_position().y;
    auto [x, y] = player_.get_position();
    common::Position noconstpos{ x,y };
    auto army = std::make_unique<Army>(
        Army(player_, noconstpos, resource_manager_.get_image("human-swordman"), 100));
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
    input_.process_cv.notify_one();
    pool.join();
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
        // TODO: This thread should only execute on army / character movements.
        // If there are no movements it should not do anything but wait.
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
            // a moving army has got a movement path as well as a movement state.
            //
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
    std::mutex event_lock;
    std::unique_lock lk(event_lock);
    LOG(INFO) << "starting event thread...";
    while (g_running)
    {
        LOG(INFO) << "event thread executing...";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// Process: is responsible for
// 1. astar calculations
// 2. acting on mouse input
//
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
            for (auto const& [id, army] : armies_)
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
                common::Position mouse_pos;
                Army* army = armies_[army_id].get();
                common::Position tile_position = get_square(map_tile_size_x_, map_tile_size_y_, army->get_position());
                mouse_to_screen_coords(event.position.x_pos, event.position.y_pos, mouse_pos);
                const auto destination = common::get_square(map_tile_size_x_, map_tile_size_y_, mouse_pos);
                if (astar_.astar(tile_position.x, tile_position.y, destination.x, destination.y, false))
                {
                    auto& result = get_astar_result();
                    auto it_pos = result.begin();
                    auto it_end = result.end();
                    while (it_pos != it_end)
                    {
                        common::BezierCurve4 b4{};
                        b4.p0 = get_square_pixel_position(*it_pos++);
                        if (it_pos != it_end)
                        {
                            b4.p1 = get_square_pixel_position(*it_pos++);
                        }
                        else
                        {
                            b4.p1 = b4.p0;
                            b4.p2 = b4.p0;
                            b4.p3 = b4.p0;
                            break;
                        }
                        if (it_pos != it_end)
                        {
                            b4.p2 = get_square_pixel_position(*it_pos++);
                        }
                        else
                        {
                            b4.p2 = b4.p1;
                            b4.p3 = b4.p1;
                            break;
                        }
                        if (it_pos != it_end)
                        {
                            b4.p3 = get_square_pixel_position(*it_pos++);

                        }
                        else
                        {
                            b4.p3 = b4.p2;
                            break;
                        }
                        army->bezier_path.add_curve(b4, 5);
                        --it_pos;
                        --it_pos;
                    }
                }
            }



            //army->set_movement_path(get_astar_result());
            input_.clear_selected_armies();
        }

    }
}

glm::vec2 Kingdom::get_square_pixel_position(int position) const
{
    const common::Position square_pos{ position % map_tile_size_y_,position / map_tile_size_x_ };
    return common::get_square_center(map_tile_size_x_, map_tile_size_y_, square_pos);
}

void Kingdom::mouse_to_screen_coords(int x, int y, common::Position& position) const
{
    const float scale_x = input_.get_scale() * static_cast<float>(window_width_) / static_cast<float>(map_width_);
    const float scale_y = input_.get_scale() * static_cast<float>(window_height_) / static_cast<float>(map_height_);
    position.x = static_cast<int>(static_cast<float>(x) / scale_x);
    position.y = static_cast<int>(static_cast<float>(y) / scale_y);
}

bool Kingdom::coords_within_square(int x, int y, const SDL_Rect& rect) const
{
    common::Position pos;
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
