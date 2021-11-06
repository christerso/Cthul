#pragma once

#include "army.h"
#include "astar.h"
#include "castle.h"
#include "character.h"
#include "input.h"
#include "player.h"
#include "resourcemanager.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <glog/logging.h>

namespace king
{
using KingdomID = std::string;
using ThreadMethod = std::function<void()>;

class Kingdom
{
public:
    Kingdom();
    Kingdom(const Kingdom& val)
    {
        LOG(ERROR) << "here";
    }
    ~Kingdom();
    void setup_kingdom();
    void set_astar_data(std::vector<int>& data);
    void movement();
    void ai();
    void weather();
    void event();
    void process();
    void spawn_new_army();
    void draw_sprites(SDL_Renderer * renderer);
    void set_input_handler(Input* input);
    const KingdomID& get_id() const;
    std::vector<Castle*> get_castles() const;
    std::map<ArmyID, std::unique_ptr<Army>>& get_armies();
    void start_threads();
    void stop_threads();

private:
    boost::asio::thread_pool pool_;
    KingdomID kingdom_id_;
    star::AStar astar_;
    Owner owner_;
    Input* input_;
    Player player_;
    std::map<CastleID, std::unique_ptr<Castle>> castles_;
    std::map<ArmyID, std::unique_ptr<Army>> armies_;
    std::map<CharacterID, std::unique_ptr<Character>> characters_;
    std::unique_ptr<ResourceManager> resource_manager_;
};
} // namespace king
