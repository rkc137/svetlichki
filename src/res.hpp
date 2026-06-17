#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <vector>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

class res
{
private:
    inline static const fs::path folder = "../res";
public:
    res() = delete;

    struct Texture : public sf::Texture
    {
        std::vector<int> animations_lengths;
        sf::Vector2i frame_size;
    };
    inline static std::vector<Texture> textures;

    static void load()
    {
        std::ifstream ifs(folder / "config.json");
        if(!ifs.is_open())
            throw std::runtime_error("failed to open config");
        auto config = json::parse(ifs);

        for(auto &image : config["images"])
        {
            Texture texture;
            if(!texture.loadFromFile(folder / image["texture_name"].get<std::string>()))
                throw std::runtime_error("failed to open texture");

            
            if(auto size = image["frame_size"]; size.size() != 2)
                throw std::runtime_error("wrong amount of arguments in frame_size");
            else
                texture.frame_size = {
                    size[0].get<int>(),
                    size[1].get<int>()
                };
            
            texture.animations_lengths = image["anim_lens"].get<std::vector<int>>();
            textures.push_back(std::move(texture));
        }
    }
private:

};