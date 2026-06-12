#include "winhandle.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({600, 600}), "svetlichki", sf::Style::None);
    auto winhandle = Winhandle{window};

    sf::Texture texture("../res/svet.png");
    sf::Sprite svet(texture);
    auto [w, h] = texture.getSize();
    svet.setScale({0.5, 0.5});
    svet.setOrigin({w / 2.f, h / 2.f});
    svet.setPosition({w / 2.f, h / 2.f});

    while(window.isOpen())
    {
        while(const auto event = window.pollEvent())
            if(event->is<sf::Event::Closed>())
                window.close();
        if(winhandle.is_should_close())
            window.close();
        static unsigned int angl = 0;
        angl++;
        svet.setRotation(sf::degrees(angl % 360));
        window.clear();
        window.draw(svet);
        window.display();
    }
    return 0;
}