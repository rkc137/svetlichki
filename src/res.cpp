#include "res.hpp"


template <typename T, typename U = T>
void set_if_find(json &json_data, T& data, std::string_view name)
{
    if(auto it = json_data.find(name); it != json_data.end())
        data = it->get<U>();
}

void res::load(sf::Vector2u wsize)
{
    window_size = wsize;
    auto [w, h] = get_wsize<float>();
    auto maxwsize = std::max(w, h);

    std::ifstream ifs(folder / "config.json");
    if(!ifs.is_open())
        throw std::runtime_error("failed to open config");
    auto config = json::parse(ifs);

    set_if_find(config, transparent, "transparent");
    set_if_find(config, is_rgba_important, "is_rgba_important");
    set_if_find(config, entitys_count, "count");
    set_if_find(config, sprite_ratio, "ratio");
    set_if_find(config, fade_ratio, "fade");
    if(!sprite_ratio || !entitys_count || !transparent)
        throw std::runtime_error("one of important variables is zero");
    tgt_sprite_size = maxwsize * sprite_ratio;
    fade_radius = maxwsize * fade_ratio;

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
