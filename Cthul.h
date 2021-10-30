// Cthul.h : Include file for standard system include files,
// or project specific include files.

#ifndef CTHUL_H
#define CTHUL_H

#include <iostream>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "ResourceManager.h"
#include "resources/entities/entities.h"

class Cthul
{
public:
    enum MOUSE_BUTTONS
    {
        LEFT = 0,
        MIDDLE = 1,
        RIGHT = 2
    };

    Cthul();

    ~Cthul();

    void initialize_SDL2();
    void shutdown_SDL2();
    void create_window();
    void start_input_loop();
    void setup_resources();

    SDL_Renderer* renderer_ = nullptr;
    SDL_Window* window_ = nullptr;
    SDL_Surface* screen_surface_ = nullptr;

private:
    RM::ResourceManager resources_;
    std::vector<bool> mouse_button_states_;
};

#undef main
int main(int argc, char* argv[])
{
    std::cout << "Starting Cthul" << std::endl;

    FLAGS_alsologtostderr = true;
    FLAGS_logbuflevel = google::GLOG_INFO;
    FLAGS_log_dir = "logging";
    FLAGS_max_log_size = 1000000;
    google::InitGoogleLogging(argv[0]);

    // parse command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    DLOG(INFO) << "Logging initialized";

    Cthul cthul;

    cthul.initialize_SDL2();
    cthul.create_window();

    cthul.setup_resources();

    cthul.start_input_loop();
    cthul.shutdown_SDL2();

    google::ShutdownGoogleLogging();

    return 0;
}

#endif // CTHUL_H