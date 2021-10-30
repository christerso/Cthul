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
struct Sprite
{
    SDL_Texture* texture  {};
    SDL_Rect rect {};
    Uint32* format {};
    int* access {};
};

using ImageStore = std::unordered_map<std::string, std::unique_ptr<Sprite>>;
using ResourceLoader = std::unordered_map<std::string, std::function<SDL_Texture*(std::string, std::string)>>;

class ResourceManager
{
public:
    // setup initial resources
    // this means load level, load everything belonging to the level etc
    void setup_initial_resources();
    void set_renderer(SDL_Renderer* renderer);

    // This load function should be generic.
    // Anything given should be handled.
    virtual SDL_Texture* load_image(const std::string name, const std::string filename, bool is_grayscale);
    virtual const Sprite& get_sprite(const std::string name);
    virtual bool load_entity(std::string name, std::string filename);

private:
    // using Freeimage
    FIBITMAP* get_freeimage_bitmap(std::string filename);
    SDL_Surface* get_sdl_surface(FIBITMAP* freeimage_bitmap, int is_greyscale);

    virtual bool load_level(std::string name);

private:
    ResourceLoader resource_loader_;
    ImageStore image_store_;
    SDL_Renderer* renderer_ = nullptr;
};
} // namespace RM


#endif // RESOURCE_MANAGER_H
