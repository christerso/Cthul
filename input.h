#pragma once
#include "army.h"

#include <queue>
#include <vector>

namespace king
{

struct MousePosition
{
    int x_pos{};
    int y_pos{};
};

using SelectedArmies = std::vector<ArmyID>;

class Input
{
public:
    void append_left_mouse_input(MousePosition& entry);
    void append_right_mouse_input(MousePosition& entry);

    SelectedArmies selected_armies;
    MousePosition current_left_mouse_entry;
    MousePosition current_right_mouse_entry;
    std::queue<MousePosition> mouse_source;
    std::queue<MousePosition> mouse_destination;
    bool left_ctrl_down = false;
    bool left_alt_down = false;
    bool left_shift_down = false;
};

}; // namespace king
