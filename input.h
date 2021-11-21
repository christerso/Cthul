#pragma once

#include "army.h"

#include <mutex>
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
    kRight = 2,
    kNone = 100
};

struct MouseQueueEvent
{
    MousePosition position;
    MouseButtons action {kNone};
};
constexpr int kScrollSpeed = 50;
constexpr int kZoomStep = 20;

class Input
{
public:
    std::mutex input_mtx;
    std::condition_variable process_cv; // condition variable for the "process" task

public:
    Input();
    ~Input();
    const SDL_Rect& get_camera() const;
    float get_scale() const;
    void set_camera(SDL_Rect& camera);
    MousePosition& scroll_wheel_mouse_position();
    void setup_world_data(int width, int height);
    bool input_loop();
    void append_left_mouse_input(MousePosition& entry);
    void append_right_mouse_input(MousePosition& entry);
    bool show_astar_path() const;
    const SelectedArmies& get_selected_armies() const;
    void add_selected_army(const ArmyID& army);
    void clear_selected_armies();
    bool get_next_mouse_action(MouseQueueEvent& event);

    std::vector<bool> mouse_button_states_;
    SDL_Rect highlight_square_;
private:
    bool left_ctrl_down_ = false;
    bool left_alt_down_ = false;
    bool left_shift_down_ = false;
    bool show_astar_path_ = false;
    SelectedArmies selected_armies_;
    std::queue<MouseQueueEvent> mouse_queue_;
    MousePosition current_right_mouse_entry_;
    MousePosition current_middle_mouse_entry_;
    MousePosition current_left_mouse_entry_;
    MousePosition scroll_wheel_mouse_position_;
    float scale_ = 1.f;
    int scroll_speed_ = kScrollSpeed;
    int world_width_ {};
    int world_height_ {};
    SDL_Rect camera_ {};
    SDL_Event sdl_event_{};


};

}; // namespace king
