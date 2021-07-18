#include "ResourceManager.h"

#include "SDL2Wrapper.h"

#include <iostream>
#include <memory>

#include <FreeImage.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/functional.hpp>
#include <fmt/format.h>
#include <glog/logging.h>


using namespace RM;

ResourceManager::ResourceManager()
{
    m_resource_loader["png"] = [&](std::string name, std::string filename) -> bool {
        return load_image(name, filename);
    };
    m_resource_loader["jpg"] = [&](std::string name, std::string filename) -> bool {
        return load_image(name, filename);
    };
    m_resource_loader["bmp"] = [&](std::string name, std::string filename) -> bool {
        return load_image(name, filename);
    };
}

ResourceManager::~ResourceManager() {}

// initialise a FreeImage bitmap and return a pointer to it.
FIBITMAP* ResourceManager::get_freeimage_bitmap(std::string filename)
{
    FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(filename.c_str(), 0);
    FIBITMAP* freeimage_bitmap = FreeImage_Load(filetype, filename.c_str(), 0);
    return freeimage_bitmap;
}

SDL_Surface* ResourceManager::get_sdl_surface(FIBITMAP* freeimage_bitmap, int is_grayscale)
{
    // loaded image is upside down, so flip it.
    FreeImage_FlipVertical(freeimage_bitmap);

    SDL_Surface* sdl_surface =
        SDL_CreateRGBSurfaceFrom(FreeImage_GetBits(freeimage_bitmap), FreeImage_GetWidth(freeimage_bitmap),
                                 FreeImage_GetHeight(freeimage_bitmap), FreeImage_GetBPP(freeimage_bitmap),
                                 FreeImage_GetPitch(freeimage_bitmap), FreeImage_GetRedMask(freeimage_bitmap),
                                 FreeImage_GetGreenMask(freeimage_bitmap), FreeImage_GetBlueMask(freeimage_bitmap), 0);

    if (sdl_surface == nullptr)
    {
        LOG(ERROR) << fmt::format("failed to create surface: {}", SDL_GetError());
        exit(1);
    }

    if (is_grayscale)
    {
        // to display a gray-scale image we need to create a custom palette.
        SDL_Color colors[256];
        for (int i = 0; i < 256; i++)
        {
            colors[i].r = colors[i].g = colors[i].b = i;
        }
        SDL_SetPaletteColors(sdl_surface->format->palette, colors, 0, 256);
    }

    return sdl_surface;
}

bool ResourceManager::load(std::string name, std::string filename)
{
    // make all input lowercase
    boost::to_lower(filename);

    // deduce file type from extension
    auto found = filename.find('.');
    std::string substring;
    if (found != std::string::npos && found > 1)
    {
        ++found;
        substring = filename.substr(found, filename.size());
    }
    else
    {
        LOG(ERROR) << "invalid extension.";
        return false;
    }

    if (m_resource_loader[substring](name, filename) != true)
    {
        LOG(ERROR) << fmt::format("unable to load resource: {} {}", name, filename);
        return false;
    }

    return true;
}

bool ResourceManager::load_image(std::string name, std::string filename)
{
    boost::filesystem::path full_path(boost::filesystem::current_path());
    std::cout << "Current path is : " << full_path << std::endl;

    auto bitmap = get_freeimage_bitmap(filename);
    if (bitmap == nullptr)
    {
        LOG(ERROR) << fmt::format("unable to load image with freeimage: {}", filename.c_str());
    }

    SDL_Surface* sdl_surface = get_sdl_surface(bitmap, true);
    if (sdl_surface == nullptr)
    {
        LOG(ERROR) << "unable to load sdl surface";
        exit(-1);
    }

   m_image_store[name] = std::make_shared<SDL_Surface>(*sdl_surface);
    LOG(INFO) << fmt::format("loaded image: {} from {}", name, filename);

    return true;
}

// read json data for images to load: TODO to be continued...
bool ResourceManager::load_level(std::string name)
{
    return true;
}
