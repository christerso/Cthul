// Chtuhl.h : Include file for standard system include files,
// or project specific include files.

#ifndef CHTHUL_H
#define CHTHUL_H

#include <iostream>

#include <fmt/format.h>
#include <glog/logging.h>

#include "ResourceManager.h"
#include "SDL2Wrapper.h"

// SDL2 adds a main definition to the environment, un-defining it here
#undef main

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

    SW::SDL2Wrapper::initialize_SDL2();

    SW::SDL2Wrapper::create_window();

    SW::SDL2Wrapper::start_input_loop();

    SW::SDL2Wrapper::shutdown_SDL2();

    google::ShutdownGoogleLogging();

    return 0;
}

#endif // CHTHUL_H