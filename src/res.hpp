#pragma once

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
    inline static sf::Vector2u window_size;
public:
    res() = delete;

    struct Texture : public sf::Texture
    {
        std::vector<int> animations_lengths;
        sf::Vector2i frame_size;
        bool is_top_down = true;
    };
    inline static std::vector<Texture> textures;
    
    template <typename T = unsigned int>
    requires (std::is_arithmetic_v<T>)
    static sf::Vector2<T> get_wsize()
    {
        return static_cast<sf::Vector2<T>>(window_size);
    }

    static float get_tgt_sprite_size();
    static void load(sf::Vector2u window_size, float ratio = .1f);
};