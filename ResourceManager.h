#ifndef RESOURCE_WRAPPER_H
#define RESOURCE_WRAPPER_H

#include <FreeImage.h>
#include <SDL2/SDL_surface.h>

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>


namespace RM
{
using ImageStore = std::unordered_map<std::string, std::shared_ptr<SDL_Surface>>;
using ResourceLoader = std::unordered_map<std::string, std::function<bool(std::string, std::string)>>;


class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    // This load function should be generic.
    // Anything given should be handled.
    virtual bool load(std::string name, std::string filename);

private:
    // using Freeimage
    FIBITMAP* get_freeimage_bitmap(std::string filename);
    SDL_Surface* get_sdl_surface(FIBITMAP* freeimage_bitmap, int is_grayscale);
    virtual bool load_image(std::string name, std::string filename);
    virtual bool load_level(std::string name);

private:
    ResourceLoader m_resource_loader;
    ImageStore m_image_store;
};


} // namespace RM


#endif // RESOURCE_WRAPPER_H
