#include "resourcemanager.h"

#include "astar.h"
#include "castle.h"

#include "rapidcsv.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/json/src.hpp>
#include <boost/json/value.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

#include <fstream>

using namespace king;

ResourceManager::ResourceManager()
{
}

void ResourceManager::setup_initial_resources()
{
    load_image("map", "resources/images/kings.jpg");
    load_image("human-axe-rider", "resources/images/human-axe-rider.png");
    load_image("human_shieldman-rider", "resources/images/human-shieldman-rider.png");
    load_image("human-spearman-rider", "resources/images/human-spearman-rider.png");
    load_image("human-swordman", "resources/images/human-swordman.png");
    load_layer_data("resources/data/kingdom_astar.csv", map_layers_.astar_data);
    load_layer_data("resources/data/kingdom_events.csv", map_layers_.map_events_areas);
    load_layer_data("resources/data/kingdom_sightline.csv", map_layers_.sightline_data);
}

void ResourceManager::set_renderer(SDL_Renderer* renderer)
{
    renderer_ = renderer;
}

MapData& ResourceManager::get_astar_data()
{
    return map_layers_.astar_data;
}

MapData& ResourceManager::get_sightline_data()
{
    return map_layers_.sightline_data;
}

MapData& ResourceManager::get_event_data()
{
    return  map_layers_.map_events_areas;
}

const Sprite& ResourceManager::load_image(const std::string& name, const std::string& filename)
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

    auto sprite = std::make_unique<Sprite>();
    SDL_QueryTexture(texture, sprite->format, sprite->access, &sprite->source_rect.w, &sprite->source_rect.h);
    sprite->texture = texture;

    image_store_[name] = std::move(sprite);
    LOG(INFO) << fmt::format("loaded image: {} from {}", name, filename);

    return *image_store_[name];
}


Sprite* ResourceManager::get_image(const std::string& name)
{
    if (image_store_.find(name) == image_store_.end())
    {
        throw std::runtime_error("image was not found");
    }
    return image_store_[name].get();
}

void ResourceManager::load_layer_data(const std::string& filename, MapData& map_data)
{
    rapidcsv::Document doc(filename, rapidcsv::LabelParams(-1, -1));
    for (auto i = 0; i < doc.GetRowCount(); i++)
    {
        std::vector<int> row = doc.GetRow<int>(i);
        for (auto& entry: row)
        {
            map_data.push_back(entry);
        }
    }
}

void ResourceManager::load_json(const std::string& name, const std::string& filename) const
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

const MapLayers& ResourceManager::get_layer_data()
{
    return map_layers_;
}