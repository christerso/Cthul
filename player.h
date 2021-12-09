#pragma once
#include "castle.h"
#include "character.h"

namespace king
{
class Player : public Character
{
public:
    Player();
    virtual ~Player();
    void create_inital_setup();
    const CastleID& get_castle() const;
    void set_castle(CastleID& castle);

private:
    CastleID castle_;
};

} // namespace king