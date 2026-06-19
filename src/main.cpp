#include <list>

#include "winhandle.hpp"
#include "Entity.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode::getFullscreenModes().front(),
        "svetlichki", sf::Style::None
    );
    window.setFramerateLimit(60);
    auto winhandle = Winhandle{window};

    res::load(window.getSize());

    std::vector<Entity> entitys;
    {
        auto [x, y] = static_cast<sf::Vector2i>(window.getSize());
        for(auto &texture : res::textures)
        for(int i = 0; i < 3; i++)
        {
            auto& e = entitys.emplace_back(texture);
            e.set_animation(i % texture.animations_lengths.size());
            sf::Vector2i pos{rand() % x, rand() % y};
            e.setPosition(static_cast<sf::Vector2f>(pos));
        }
    }

    while(window.isOpen())
    {
        while(const auto event = window.pollEvent())
            if(event->is<sf::Event::Closed>())
                window.close();
        if(winhandle.is_should_close())
            window.close();
        window.clear();
        for(auto &e : entitys)
        {
            e.update();
            window.draw(e);
        }
        window.display();
    }
    return 0;
}