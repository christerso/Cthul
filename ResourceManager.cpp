#include "resourcemanager.h"

#include "astar.h"
#include "castle.h"

#include "rapidcsv.h"
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
    load_image("map", "resources/images/kings.jpg", false, false);
    load_image("human-axe-rider", "resources/images/human-axe-rider.png", true, true);
    load_image("human_shieldman-rider", "resources/images/human-shieldman-rider.png", true, true);
    load_image("human-spearman-rider", "resources/images/human-spearman-rider.png", true, true);
    load_image("human-swordman", "resources/images/human-swordman.png", true, true);
    load_astar_data("resources/data/kingdom_astar.csv");
}

void ResourceManager::set_renderer(SDL_Renderer* renderer)
{
    renderer_ = renderer;
}

const MapData& ResourceManager::get_astar_data() const
{
    return astar_data_;
}

const Sprite& ResourceManager::load_image(const std::string& name, const std::string& filename, bool entity,
                                          bool is_grayscale)
{
    SDL_Surface* image = IMG_Load(filename.c_str());
    if (image == nullptr)
    {
        LOG(INFO) << fmt::format("unable to load image: {} SDL Error {}", filename.c_str(), SDL_GetError());
        throw std::runtime_error(
            fmt::format("unable to load image: {} SDL Error {}", filename.c_str(), SDL_GetError()));
    }

    // create texture from surface pixels
    const auto texture = SDL_CreateTextureFromSurface(renderer_, image);
    if (texture == nullptr)
    {
        throw std::runtime_error(
            fmt::format("unable to create texture from {}! SDL Error: {}", filename.c_str(), SDL_GetError()));
    }
    SDL_FreeSurface(image);

    std::unique_ptr<Sprite> sprite(new Sprite);
    SDL_QueryTexture(texture, sprite->format, sprite->access, &sprite->source_rect.w, &sprite->source_rect.h);
    sprite->texture = texture;
    if (entity)
    {
        entity_store_[name] = std::move(sprite);
    }
    else
    {
        image_store_[name] = std::move(sprite);
    }
    LOG(INFO) << fmt::format("loaded image: {} from {}", name, filename);

    return *image_store_[name];
}


const Sprite& ResourceManager::get_sprite(const std::string& name)
{
    if (image_store_.find(name) == image_store_.end())
    {
        throw std::runtime_error("image was not found");
    }
    return *image_store_[name];
}

const EntityStore& ResourceManager::get_entities() const
{
    return entity_store_;
}

void ResourceManager::load_astar_data(const std::string& filename)
{
    rapidcsv::Document doc(filename, rapidcsv::LabelParams(-1, -1));
    for (auto i = 0; i < doc.GetRowCount(); i++)
    {
        astar_data_.emplace_back(doc.GetRow<int>(i));
    }
}

void ResourceManager::load_json(const std::string& name, const std::string& filename)
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
    }
}
