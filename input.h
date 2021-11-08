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

enum MouseButtons
{
    kLeft = 0,
    kMiddle = 1,
    kRight = 2
};

class Input
{
public:
    Input();
    ~Input();
    const SDL_Rect& get_position() const;
    void setup_world_data(int width, int height);
    bool input_loop();
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

private:
    int scroll_speed_ = 50;
    int world_width_ {};
    int world_height_ {};
    SDL_Rect camera_ {};
    SDL_Event sdl_event_{};
    SDL_Rect position_ {};
    std::vector<bool> mouse_button_states_;
};

}; // namespace king
