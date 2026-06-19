#pragma once

#include <SFML/Graphics.hpp>

#include "res.hpp"

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity(const res::Texture &texture);
    ~Entity() = default;

    void update();
    void set_animation(int animation);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
    void animation_update();

private:
    const res::Texture &texture;
    bool is_sprite_reverse = false;
    sf::Sprite sprite;
    
    sf::Clock anim_clock;
    sf::Time anim_delta_time = sf::seconds(0.1);
    int anim_index = 0;
    unsigned int anim_step = 0;

    enum class State {
        rest,
        moving
    } state;
    sf::Vector2f tgt_point{1.f, 1.f};
};