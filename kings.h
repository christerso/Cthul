// Cthul.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "kingdom.h"

#include <iostream>
#include <random>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "resourcemanager.h"

namespace king
{

class Kings
{
public:
    enum MOUSE_BUTTONS
    {
        LEFT = 0,
        MIDDLE = 1,
        RIGHT = 2
    };

    Kings();
    ~Kings();

    void start();
    void initialize_sdl2();
    void shutdown_sdl2();
    void create_window();
    void start_input_loop();
    void setup_resources();
    void setup_kingdom();
    void draw_sprites();
    Kingdom& get_kingdom() const;

private:
    ThreadManager thread_manager_;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Window* window_ = nullptr;
    SDL_Surface* screen_surface_ = nullptr;
    std::unique_ptr<Kingdom> kingdom_;
    RM::ResourceManager resources_;
    std::vector<bool> mouse_button_states_;
};

} // namespace king

int main(int argc, char* argv[])
{
    std::cout << "starting kings" << std::endl;

    FLAGS_alsologtostderr = true;
    FLAGS_logbuflevel = google::GLOG_INFO;
    FLAGS_log_dir = "logging";
    FLAGS_max_log_size = 1000000;
    google::InitGoogleLogging(argv[0]);

    // parse command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    DLOG(INFO) << "logging initialized";

    king::Kings kings;
    kings.start();
    google::ShutdownGoogleLogging();

    return 0;
}
