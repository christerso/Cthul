#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <glog/logging.h>
/*
#define NANOSVG_ALL_COLOR_KEYWORDS // Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION     // Expands implementation
#include "nanosvg.h"
*/
namespace king
{
struct Sprite
{
    SDL_Texture* texture{};
    SDL_Rect source_rect{};
    SDL_Point size;
    Uint32* format{};
    int* access{};
};

using ImageStore = std::unordered_map<std::string, std::unique_ptr<Sprite>>;
using ResourceLoader = std::unordered_map<std::string, std::function<SDL_Texture*(std::string, std::string)>>;
using MapData = std::vector<int>;
class ResourceManager
{
public:
    ResourceManager();

    // setup initial resources
    // this means load level, load everything belonging to the level etc
    void setup_initial_resources();
    void set_renderer(SDL_Renderer* renderer);
    MapData& get_astar_data();

    // This load function should be generic.
    // Anything given should be handled.
    const Sprite& load_image(const std::string& name, const std::string& filename);
    Sprite* get_image(const std::string& name);
    void load_json(const std::string& name, const std::string& filename) const;
    void load_astar_data(const std::string& filename);
private:
    ResourceLoader resource_loader_;
    ImageStore image_store_;
    SDL_Renderer* renderer_ = nullptr;
    MapData astar_data_;
};
} // namespace king


#endif // RESOURCE_MANAGER_H
