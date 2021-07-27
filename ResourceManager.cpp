#include "ResourceManager.h"

#include <FreeImage.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/functional.hpp>
#include <boost/json/src.hpp>
#include <boost/json/value.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

#include <fstream>

using namespace RM;

ResourceManager::ResourceManager()
{
    
}

ResourceManager::~ResourceManager()
{
    // destroyed in cthul class
    m_renderer = nullptr;
}

void ResourceManager::setup_initial_resources()
{
    load_image("Adam", "resources/images/human.png");
}

void ResourceManager::set_renderer(SDL_Renderer* renderer)
{
    m_renderer = renderer;
}

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

SDL_Texture* ResourceManager::load_image(std::string name, std::string filename)
{
    auto bitmap = get_freeimage_bitmap(filename);
    if (bitmap == nullptr)
    {
        LOG(ERROR) << fmt::format("unable to load image with freeimage: {}", filename.c_str());
        return nullptr;
    }

    auto sdl_surface = get_sdl_surface(bitmap, true);
    if (sdl_surface == nullptr)
    {
        LOG(ERROR) << "unable to load sdl surface";
        return nullptr;
    }

    // create texture from surface pixels
    auto texture = SDL_CreateTextureFromSurface(m_renderer, sdl_surface);
    if (texture == nullptr)
    {
        LOG(ERROR) << fmt::format("unable to create texture from {}! SDL Error: {}", filename.c_str(), SDL_GetError());
    }
    SDL_FreeSurface(sdl_surface);
    m_image_store[name] = texture;
    LOG(INFO) << fmt::format("loaded image: {} from {}", name, filename);

    return texture;
}

SDL_Texture* ResourceManager::grab_random_wallpaper_image()
{
    return nullptr;
}

SDL_Texture* ResourceManager::get_image(std::string image)
{
    if (m_image_store.find(image) == m_image_store.end())
    {
        LOG(ERROR) << fmt::format("image not found in image store {}", image);
        return nullptr;
    }
    return m_image_store[image];
}

// read json data for images to load: TODO to be continued...
bool ResourceManager::load_level(std::string name)
{
    return true;
}

// all entities have unique names
bool ResourceManager::load_entity(std::string name, std::string filename)
{
    if (std::ifstream file(filename, std::fstream::in); file.is_open())
    {
        std::string out;
        std::string line;
        while (std::getline(file, line))
        {
            out.append(line);
        }
        LOG(INFO) << out;
        file.close();

        try
        {
            boost::json::error_code ec;
            boost::json::value v = boost::json::parse(out, ec);
            if (ec)
            {
                LOG(ERROR) << fmt::format("parsing of {} failed", filename);
                exit(-1);
            }
        }
        catch (std::bad_alloc const& e)
        {
            LOG(ERROR) << "parsing failed: " << e.what() << std::endl;
            exit(-1);
        }
        return true;
    }
    return false;
}
