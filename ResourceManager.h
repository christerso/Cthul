#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>

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
struct [[nodiscard]] Sprite
{
    SDL_Texture* texture{};
    SDL_Rect source_rect{};
    SDL_FRect destination_rect{};
    Uint32* format{};
    int* access{};
};

using ImageStore = std::unordered_map<std::string, std::unique_ptr<Sprite>>;
using EntityStore = std::unordered_map<std::string, std::unique_ptr<Sprite>>;
using ResourceLoader = std::unordered_map<std::string, std::function<SDL_Texture*(std::string, std::string)>>;
using MapData = std::vector<std::vector<int>>;
class ResourceManager
{
public:
    // setup initial resources
    // this means load level, load everything belonging to the level etc
    void setup_initial_resources();
    void set_renderer(SDL_Renderer* renderer);
    const MapData& get_astar_data() const;

    // This load function should be generic.
    // Anything given should be handled.
    const Sprite& load_image(const std::string& name, const std::string& filename, bool entity,
                                     bool is_greyscale);
    const Sprite& get_sprite(const std::string& name);
    const EntityStore& get_entities() const;
    void load_json(const std::string& name, const std::string& filename);
    void load_astar_data(const std::string& filename);
private:
    ResourceLoader resource_loader_;
    ImageStore image_store_;
    EntityStore entity_store_;
    SDL_Renderer* renderer_ = nullptr;
    MapData astar_data_;
};
} // namespace RM


#endif // RESOURCE_MANAGER_H
