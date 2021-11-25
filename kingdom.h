#pragma once

#include "army.h"
#include "astar.h"
#include "castle.h"
#include "character.h"
#include "input.h"
#include "player.h"
#include "render.h"
#include "resourcemanager.h"
#include "timer.h"

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <boost/random.hpp>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <glm/vec2.hpp>

namespace king
{

enum class Level
{
    kGround = 0,
    kUnderground,
    kAir,
    kWater
};

using KingdomID = std::string;
using ThreadMethod = std::function<void()>;
using CastleIDs = std::vector<CastleID>;

static bool g_running;
class Kingdom
{
public:
    Kingdom();
    ~Kingdom();
    void run();
    void setup_kingdom();
    void set_astar_data(std::vector<int>& data);
    void movement();
    void ai();
    void weather();
    void event();
    void process();
    glm::vec2 get_square_pixel_position(int position) const;
    void mouse_to_screen_coords(int x, int y, glm::vec2& position) const;
    bool coords_within_square(int x, int y, const SDL_Rect& rect) const;
    CastleID& get_castle_id(int position);
    void spawn_new_army();
    Input& get_input();
    void draw_bezier_paths(SDL_Renderer* renderer) const;
    void draw_sprites(SDL_Renderer* renderer) const;
    const KingdomID& get_id() const;
    const Castle& get_castle(const CastleID& id) const;
    std::map<ArmyID, std::unique_ptr<Army>>& get_armies();
    void start_threads();
    void stop_threads();
    std::vector<int>& get_astar_result();
    int get_tile_sizes() const;
private:
    bool right_mouse_action_ = false;
    bool left_mouse_action_ = false;
    CastleIDs ids_;
    boost::asio::thread_pool pool;
    KingdomID kingdom_id_;
    star::AStar astar_;
    Owner owner_;
    Input input_;
    Timer& timer_;
    Player player_;
    std::map<CastleID, std::unique_ptr<Castle>> castles_;
    std::map<ArmyID, std::unique_ptr<Army>> armies_;
    std::map<CharacterID, std::unique_ptr<Character>> characters_;
    ResourceManager resource_manager_;
    Render render_;
    int map_tile_size_x_ {};
    int map_tile_size_y_ {};
    int window_width_ {};
    int window_height_ {};
    int map_height_ {};
    int map_width_ {};
    

};
} // namespace king
