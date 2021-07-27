#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL2/SDL.h>
#undef main
#include <FreeImage.h>
#include <functional>
#include <string>
#include <unordered_map>
/*
#define NANOSVG_ALL_COLOR_KEYWORDS // Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION     // Expands implementation
#include "nanosvg.h"
*/
namespace RM
{
using ImageStore = std::unordered_map<std::string, SDL_Texture*>;

using ResourceLoader = std::unordered_map<std::string, std::function<SDL_Texture*(std::string, std::string)>>;

class ResourceManager
{
public:
    ResourceManager();
     ~ResourceManager();

    // setup initial resources
    // this means load level, load everything belonging to the level etc
    void setup_initial_resources();
    void set_renderer(SDL_Renderer* renderer);
    // This load function should be generic.
    // Anything given should be handled.
    virtual SDL_Texture* load_image(std::string name, std::string filename);
    SDL_Texture* grab_random_wallpaper_image();
    virtual SDL_Texture* get_image(std::string image);
    virtual bool load_entity(std::string name, std::string filename);

private:
    // using Freeimage
    FIBITMAP* get_freeimage_bitmap(std::string filename);
    SDL_Surface* get_sdl_surface(FIBITMAP* freeimage_bitmap, int is_grayscale);

    virtual bool load_level(std::string name);

private:
    ResourceLoader m_resource_loader;
    ImageStore m_image_store;
    SDL_Renderer* m_renderer;
};
} // namespace RM


#endif // RESOURCE_MANAGER_H
