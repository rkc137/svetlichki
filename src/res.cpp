#include "res.hpp"


void res::load(sf::Vector2u wsize)
{
    window_size = wsize;
    auto [w, h] = get_wsize<float>();

    std::ifstream ifs(folder / "config.json");
    if(!ifs.is_open())
        throw std::runtime_error("failed to open config");
    auto config = json::parse(ifs);

    if(auto it = config.find("is_rgba_important"); it != config.end())
        is_rgba_important = it->get<bool>();

    if(auto it = config.find("ratio"); it != config.end())
        sprite_ratio = it->get<float>();
    tgt_sprite_size = std::max(w, h) * sprite_ratio;

    if(auto it = config.find("count"); it != config.end())
        entitys_count = it->get<float>();

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

        bool is_pixel = false;
        if(auto it = image.find("is_pixel"); it != image.end())
            is_pixel = it->get<bool>();
        texture.setSmooth(!is_pixel);

        if(auto it = image.find("is_top_down"); it != image.end())
            texture.is_top_down = it->get<bool>();

        textures.push_back(std::move(texture));
    }
}
