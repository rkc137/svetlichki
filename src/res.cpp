#include "res.hpp"


void res::load(sf::Vector2u wsize, float ratio)
{
    window_size = wsize;
    auto [w, h] = static_cast<sf::Vector2f>(window_size);
    tgt_sprite_size = std::max(w, h) * ratio;

    std::ifstream ifs(folder / "config.json");
    if(!ifs.is_open())
        throw std::runtime_error("failed to open config");
    auto config = json::parse(ifs);

    if(auto it = config.find("is_rgba_important"); it != config.end())
        is_rgba_important = it->get<bool>();

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

        if(auto it = image.find("is_top_down"); it != image.end())
            texture.is_top_down = it->get<bool>();
        
        textures.push_back(std::move(texture));
    }
}
