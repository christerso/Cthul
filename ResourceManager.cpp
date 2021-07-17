#include "ResourceManager.h"

#include "SDL2Wrapper.h"

#include <iostream>
#include <memory>

#include <SDL2/SDL_image.h>

#include <boost/algorithm/string.hpp>
#include <boost/functional.hpp>
#include <boost/filesystem.hpp>
#include <fmt/format.h>
#include <glog/logging.h>
#include <rapidjson/rapidjson.h>

using namespace RM;

ResourceManager::ResourceManager()
{
    m_resource_loader["png"] = [&](std::string name, std::string filename)->bool { return load_image(name, filename); };
    m_resource_loader["jpg"] = [&](std::string name, std::string filename)->bool { return load_image(name, filename); };
    m_resource_loader["bmp"] = [&](std::string name, std::string filename)->bool { return load_image(name, filename); };
}

ResourceManager::~ResourceManager()
{
    
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
    auto image = IMG_Load(filename.c_str());
    if (image == nullptr)
    {
        LOG(ERROR) << fmt::format("unable to load image: {}", filename);
        exit(-1);
    }

    m_image_store[name] = std::make_shared<SDL_Surface>(*image);
    LOG(INFO) << fmt::format("loaded image: {} from {}", name, filename);

    return true;
}

// read json data for images to load: TODO to be continued...
bool ResourceManager::load_level(std::string name)
{
    return true;
}
