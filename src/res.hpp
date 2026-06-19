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
    inline static float tgt_sprite_size = 0.f;
public:
    res() = delete;

    struct Texture : public sf::Texture
    {
        std::vector<int> animations_lengths;
        sf::Vector2i frame_size;
    };
    inline static std::vector<Texture> textures;

    static float get_tgt_sprite_size()
    {
        return tgt_sprite_size;
    }

    static void load(sf::Vector2u window_size, float ratio = .1f)
    {
        auto [w, h] = window_size;
        tgt_sprite_size = std::max(w, h) * ratio;

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
            
            bool is_smooth = true;
            if(auto it = image.find("is_pixel"); it != image.end())
                is_smooth = it->get<bool>();
            texture.setSmooth(is_smooth);
            
            textures.push_back(std::move(texture));
        }
    }
};