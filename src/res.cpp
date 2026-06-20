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

    if(auto images = config.find("images"); images == config.end() || images->size() == 0)
        throw std::runtime_error("images is empty");
    else for(auto &image : *images)
    {
        auto &texture = textures.emplace_back();

        if(!texture.loadFromFile(folder / image["texture_name"].get<std::string>()))
            throw std::runtime_error("failed to open texture");

        if(auto size = image.find("frame_size"); size == image.end() || size->size() != 2)
            throw std::runtime_error("bad frame_size");
        else
            texture.frame_size = {
                size->at(0).get<int>(),
                size->at(1).get<int>()
            };

        if(auto al = image.find("anim_lens"); al == image.end() || al->empty())
            throw std::runtime_error("bad anim_lens");
        else
           texture.animations_lengths = al->get<std::vector<int>>();

        bool is_pixel = false;
        if(auto it = image.find("is_pixel"); it != image.end())
            is_pixel = it->get<bool>();
        texture.setSmooth(!is_pixel);

        if(auto it = image.find("is_top_down"); it != image.end())
            texture.is_top_down = it->get<bool>();
    }
}
