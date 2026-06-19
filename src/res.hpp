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

    static float get_tgt_sprite_size() const;
    static void load(sf::Vector2u window_size, float ratio = .1f);
};