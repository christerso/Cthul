#ifndef RESOURCE_WRAPPER_H
#define RESOURCE_WRAPPER_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include <SDL2/SDL_surface.h>


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
    virtual bool load_image(std::string name, std::string filename);
    virtual bool load_level(std::string name);

private:
    ResourceLoader m_resource_loader;
    ImageStore m_image_store;
};


} // namespace RM


#endif // RESOURCE_WRAPPER_H
