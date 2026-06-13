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
public:
    res() = delete;

    static void load()
    {
        std::ifstream ifs("res/config.json");
        auto config = json::parse(std::string(std::istreambuf_iterator<char>(ifs), {}));
        for(auto &image : config["images"])
        {
            Texture texture;
            texture.loadFromFile(image.get<std::string>());
            texture.frame_size = {
                image["frame_size"][0].get<int>(),
                image["frame_size"][1].get<int>()
            };
            texture.animations_lengths = image["anim_lens"].get<std::vector<int>>();
        }
    }

    struct Texture : public sf::Texture
    {
        std::vector<int> animations_lengths;
        sf::Vector2i frame_size;
    };

    static std::vector<Texture> textures;
private:

};