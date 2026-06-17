#include "winhandle.hpp"
#include "res.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode::getFullscreenModes().front(),
        "svetlichki", sf::Style::None
    );
    window.setFramerateLimit(60);
    auto winhandle = Winhandle{window};

    res::load();

    sf::Texture texture("../res/svet.png");
    sf::Sprite svet(texture);
    {
        auto [w, h] = texture.getSize();
        svet.setOrigin({w / 2.f, h / 2.f});
        // auto scale = std::max(w, h)
        svet.setScale({0.5, 0.5});
        svet.setPosition({100, 100});
    }

    while(window.isOpen())
    {
        while(const auto event = window.pollEvent())
            if(event->is<sf::Event::Closed>())
                window.close();
        if(winhandle.is_should_close())
            window.close();
        window.clear();
        window.draw(svet);
        window.display();
    }
    return 0;
}