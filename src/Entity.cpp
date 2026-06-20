#include "Entity.hpp"

Entity::Entity(const res::Texture &texture)
 : texture(texture), sprite(texture, {{0, 0}, {texture.frame_size.x, texture.frame_size.y}})
{
    auto [x, y] = texture.frame_size;
    auto s = res::tgt_sprite_size / std::max(x, y);
    auto [wx, wy] = res::get_wsize<float>();

    sprite.setColor({255, 255, 255, res::transparent});
    sprite.setOrigin(sf::Vector2f(x / 2, y / 2));
    sprite.setScale({s, s});
    state = State::moving;
    tgt_point = { get_rand(wx), get_rand(wy) };
    setPosition({ get_rand(wx), get_rand(wy) });
}

void Entity::update()
{
    switch(state)
    {
    case State::perdesh:
    case State::rest:
        if(resting_clock.getElapsedTime() < resting_time)
            break;
        tgt_point = {
            get_rand(res::get_wsize<float>().x),
            get_rand(res::get_wsize<float>().y)
        };
        state = State::moving;
        moving_clock.restart();
    break;
    case State::moving:
    {
        auto tgt = tgt_point - getPosition();
        if(tgt.length() < 1.f)
        {
            resting_clock.restart();
            static constexpr std::array idle_states = {State::perdesh, State::rest};
            state = idle_states[get_rand(idle_states.size() - 1)];
            resting_time = sf::seconds(
                moving_clock.getElapsedTime().asSeconds() / 2 +
                get_rand(1, 10)
            );
        }
        move(tgt.normalized() * 1.f);
        if(texture.is_top_down)
            sprite.setRotation(tgt.angle() + sf::degrees(90));
        else
            is_sprite_reverse = tgt.x < 0;
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

    if(anim_clock.getElapsedTime() < anim_time)
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
