// Chtuhl.cpp : Defines the entry point for the application.
//

#include "Cthul.h"

#include "ResourceManager.h"
#include "SDL2Wrapper.h"

using namespace RM;

int main(int argc, char* argv[])
{
    std::cout << "Starting Chthul" << std::endl;

    FLAGS_alsologtostderr = true;
    FLAGS_logbuflevel = google::GLOG_INFO;
    FLAGS_log_dir = "out";
    FLAGS_max_log_size = 1000000;
    google::InitGoogleLogging(argv[0]);

    // parse command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    DLOG(INFO) << "Logging initialized";

    ResourceManager r;
    r.load("Globe", "globe.png");
    SW::SDL2Wrapper::initialize_SDL2();

    SW::SDL2Wrapper::create_window();

    SW::SDL2Wrapper::start_input_loop();

    SW::SDL2Wrapper::shutdown_SDL2();

    google::ShutdownGoogleLogging();

    return 0;
}
