#pragma once

#include <map>
#include <string>
#include <memory>
#include "army.h"
#include "castle.h"
#include "character.h"
#include "player.h"
#include "threadmanager.h"

namespace king
{
using KingdomID = std::string;

class Kingdom
{
public:
    explicit Kingdom(ThreadManager& thread_manager);
    Kingdom(const Kingdom&) = default;
    Kingdom& operator=(const Kingdom&) = default;
    Kingdom(Kingdom&& other) noexcept = default;
    Kingdom& operator=(Kingdom&& other) noexcept = default;
    ~Kingdom();
    void setup_kingdom();
    void movement();
    void ai();
    void weather();
    void event();
    void spawn_new_army();
    const KingdomID& get_id() const;
    std::vector<Castle*> get_castles() const;

private:
    Owner owner_;
    std::unique_ptr<Player> player_;
    KingdomID kingdom_id_;
    ThreadManager& thread_manager_;
    std::map<CastleID, std::unique_ptr<Castle>> castles_;
    std::map<ArmyID, std::unique_ptr<Army>> armies_;
    std::map<CharacterID, std::unique_ptr<Character>> characters_;
};
} // namespace king
