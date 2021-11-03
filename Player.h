#pragma once
#include "castle.h"
#include "character.h"

namespace king
{
class Player : public Character
{
public:
    explicit Player(Castle* castle);
    virtual ~Player();
    void create_inital_setup();
    Castle* get_castle() const;
    void set_castle(Castle* castle);

private:
    Castle* castle_ = nullptr;
};

} // namespace king