#ifndef SDL2_WRAPPER_H
#define SDL2_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include <glog/logging.h>
#include <memory>

namespace SW
{
class SDL2Wrapper
{
public:
    static void initialize_SDL2();
    static void shutdown_SDL2();
    static void create_window();
    static void start_input_loop();
};

// serializable data
struct ScreenData
{
    int m_screen_width;
    int m_screen_height;
};

}

#endif // SDL2_WRAPPER_H