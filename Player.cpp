#include "player.h"
#include "common.h"

using namespace king;
// Decide which castle belongs to player
// Create initial army and commanders.
// Spawn in sprites for army.
// Add a* movement
// Implement events that occurs as an army is moving
// Add weather

Player::Player(Castle* castle)
    : Character(castle->get_position()) // players always starts in a castle
    , castle_(castle)
{
    castle_->set_owner(this->get_id());
}

Player::~Player()
{
    castle_ = nullptr; // original ownership of all castles is the kingdom, not the player
}

void Player::create_inital_setup() {}

Castle* Player::get_castle() const
{
    // Create the names of all the people around you, these will be:
    // Adjutant, Knight (horses), Commander (ground forces), Scout1 and scout2.
    // Queen, Daughters, Sons, Pet.
    // TODO: Create an entire village of people with interactions.
    return castle_;
}

void Player::set_castle(Castle* castle)
{
    castle_ = castle;
}