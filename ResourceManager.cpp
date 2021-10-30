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

void ResourceManager::setup_initial_resources()
{
    load_image("map", "resources/images/kings.jpg", false);
    load_image("human-axe-rider", "resources/images/human-axe-rider.png", true);
    load_image("human_shieldman-rider", "resources/images/human-shieldman-rider.png", true);
    load_image("human-spearman-rider", "resources/images/human-spearman-rider.png", true);
    load_image("human-swordman", "resources/images/human-swordman.png", true);
}

void ResourceManager::set_renderer(SDL_Renderer* renderer)
{
    renderer_ = renderer;
}

// initialise a FreeImage bitmap and return a pointer to it.
FIBITMAP* ResourceManager::get_freeimage_bitmap(std::string filename)
{
    FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(filename.c_str(), 0);
    FIBITMAP* freeimage_bitmap = FreeImage_Load(filetype, filename.c_str(), 0);
    return freeimage_bitmap;
}

SDL_Surface* ResourceManager::get_sdl_surface(FIBITMAP* freeimage_bitmap, int is_greyscale)
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

    if (is_greyscale)
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

SDL_Texture* ResourceManager::load_image(const std::string name, const std::string filename, bool is_grayscale)
{
    const auto bitmap = get_freeimage_bitmap(filename);
    if (bitmap == nullptr)
    {
        LOG(ERROR) << fmt::format("unable to load image with freeimage: {}", filename.c_str());
        return nullptr;
    }

    const auto sdl_surface = get_sdl_surface(bitmap, is_grayscale);
    if (sdl_surface == nullptr)
    {
        LOG(ERROR) << "unable to load sdl surface";
        return nullptr;
    }

    // create texture from surface pixels
    const auto texture = SDL_CreateTextureFromSurface(renderer_, sdl_surface);
    if (texture == nullptr)
    {
        LOG(ERROR) << fmt::format("unable to create texture from {}! SDL Error: {}", filename.c_str(), SDL_GetError());
    }
    SDL_FreeSurface(sdl_surface);

    std::unique_ptr<Sprite> sprite(new Sprite);
    SDL_QueryTexture(texture, sprite->format, sprite->access, &sprite->rect.w, &sprite->rect.h);
    sprite->texture = texture;
    image_store_[name] = std::move(sprite);

    LOG(INFO) << fmt::format("loaded image: {} from {}", name, filename);

    return texture;
}


const Sprite& ResourceManager::get_sprite(const std::string name)
{
    if (image_store_.find(name) == image_store_.end())
    {
        LOG(ERROR) << fmt::format("image not found in image store {}", name);
        throw std::runtime_error("image was not found");
    }
    return *image_store_[name];
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
