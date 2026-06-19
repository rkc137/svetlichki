#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <vector>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#define inlstc inline static

class res
{
private:
    inlstc const fs::path folder = "../res";
    inlstc float tgt_sprite_size = 0.f;
    inlstc bool is_rgba_important = true;
    inlstc sf::Vector2u window_size;
public:
    res() = delete;

    struct Texture : public sf::Texture
    {
        std::vector<int> animations_lengths;
        sf::Vector2i frame_size;
        bool is_top_down = true;
    };
    inlstc std::vector<Texture> textures;
    
    template <typename T = unsigned int>
    requires (std::is_arithmetic_v<T>)
    static sf::Vector2<T> get_wsize()
    {
        return static_cast<sf::Vector2<T>>(window_size);
    }
    inlstc const auto& gtgt_sprite_size = tgt_sprite_size;
    inlstc const auto& gis_rgba_important = is_rgba_important;

    static void load(sf::Vector2u window_size, float ratio = .1f);
};

#undef inlstc