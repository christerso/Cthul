// Cthul.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "kingdom.h"

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <fmt/format.h>
#include "input.h"

namespace king
{

class Kings
{
public:
    Kings();
    ~Kings();

    void start();
    void initialize_sdl2();
    void shutdown_sdl2();
    Kingdom& get_kingdom();

private:
    Kingdom kingdom_;
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
