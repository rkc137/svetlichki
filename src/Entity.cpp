#include "Entity.hpp"

Entity::Entity(const res::Texture &texture)
 : texture(texture), sprite(texture, {{0, 0}, {texture.frame_size.x, texture.frame_size.y}})
{
    auto [x, y] = texture.frame_size;
    sprite.setOrigin(sf::Vector2f(x / 2, y / 2));
    auto s = res::get_tgt_sprite_size() / std::max(x, y);
    sprite.setScale({s, s});
    state = State::moving;
    set_animation(0);
}

void Entity::update()
{
    switch(state)
    {
    case State::rest:
        tgt_point = {
            static_cast<float>(rand() % res::get_wsize<int>().x),
            static_cast<float>(rand() % res::get_wsize<int>().y)
        };
        state = State::moving;
    break;
    case State::moving:
    {
        auto tgt = tgt_point - getPosition();
        if(tgt.length() < 1.f)
            state = State::rest;
        move(tgt.normalized() * 1.f);
        if(texture.is_top_down)
            sprite.setRotation(tgt.angle() + sf::degrees(90));
        break;
    }
    default:
    }
    set_animation(static_cast<int>(state) % texture.animations_lengths.size());
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
#ifndef NDEBUG
            throw std::runtime_error("animations error");
#else
            std::cerr << "animations error\n";
#endif
        }
        anim_index = 0;
        return;
    }

    anim_index = animation;
}
