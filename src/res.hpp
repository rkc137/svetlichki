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
    inlstc sf::Vector2u window_size;
public:
    inlstc float tgt_sprite_size = 0.f;
    inlstc float sprite_ratio = .1f;
    inlstc bool is_rgba_important = true;
    inlstc int entitys_count = 3;
    inlstc std::uint8_t transparent = 255;

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

    static void load(sf::Vector2u window_size);
};

#undef inlstc