#pragma once

#include <SFML/Graphics.hpp>

#include "res.hpp"
#include "get_rand.hpp"

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity(const res::Texture &texture);
    ~Entity() = default;

    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
    void animation_update();
    void set_animation(int animation);

private:
    const res::Texture &texture;
    bool is_sprite_reverse = false;
    sf::Sprite sprite;

    sf::Clock anim_clock;
    sf::Clock resting_clock;
    sf::Clock moving_clock;
    sf::Time anim_time = sf::seconds(0.1);
    sf::Time resting_time = sf::seconds(3.5);
    int anim_index = 0;
    unsigned int anim_step = 0;

    enum class State {
        rest,
        perdesh,
        moving
    } state;
    sf::Vector2f tgt_point{1.f, 1.f};
};