#include "Entity.hpp"

Entity::Entity(const res::Texture &texture)
 : texture(texture), sprite(texture, {{0, 0}, {texture.frame_size.x, texture.frame_size.y}})
{
    sprite.setOrigin(sf::Vector2f(texture.frame_size.x / 2, texture.frame_size.y / 2));
    sprite.setScale({2.5, 2.5});
    set_animation(0);
}

void Entity::update()
{
    move({
        (rand() % 20 - 10) / 10.f,
        (rand() % 20 - 10) / 10.f
    });
    animation_update();
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(sprite, states);
}

void Entity::animation_update()
{
    const auto& [fx, fy] = texture.frame_size;
    const auto &al = texture.animations_lengths;

    if(anim_clock.getElapsedTime() < anim_delta_time)
        return;
    sprite.setTextureRect({
        {
            fx * (static_cast<int>(anim_step++) % al[anim_index]) + (fx * is_sprite_reverse),
            fy * anim_index
        },
        {
            fx - (fx * is_sprite_reverse * 2),
            fy
        }
    });
    anim_clock.restart();
}

void Entity::set_animation(int animation)
{
    if(0 > animation || animation >= texture.animations_lengths.size())
    {
        if(static bool is_pass_error_msg = true; is_pass_error_msg)
        {
            is_pass_error_msg = false;
            std::cerr << "animations error\n";
        }
        anim_index = 0;
    }

    anim_index = animation;
}
