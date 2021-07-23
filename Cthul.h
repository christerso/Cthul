// Cthul.h : Include file for standard system include files,
// or project specific include files.

#ifndef CTHUL_H
#define CTHUL_H

#include <iostream>

#include <fmt/format.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "ResourceManager.h"
#include "SDL2Wrapper.h"
#include "resources/entities/entities.h"

// SDL2 adds a main definition to the environment, un-defining it here
#undef main

class Cthul
{
public:
    Cthul() = default;
    ~Cthul() = default;

    void setup();

private:
    RM::ResourceManager rm;
};


int main(int argc, char* argv[])
{
    std::cout << "Starting Chthul" << std::endl;

    FLAGS_alsologtostderr = true;
    FLAGS_logbuflevel = google::GLOG_INFO;
    FLAGS_log_dir = "logging";
    FLAGS_max_log_size = 1000000;
    google::InitGoogleLogging(argv[0]);

    // parse command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    DLOG(INFO) << "Logging initialized";

    Cthul cthul;

    SW::SDL2Wrapper::initialize_SDL2();
    SW::SDL2Wrapper::create_window();

    cthul.setup();

    SW::SDL2Wrapper::start_input_loop();
    SW::SDL2Wrapper::shutdown_SDL2();

    google::ShutdownGoogleLogging();

    return 0;
}

#endif // CTHUL_H