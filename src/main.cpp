#include <list>

#include "RWindow.hpp"
#include "Entity.hpp"

// int main();
// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) { return main(); }

int run()
{
    FreeConsole();

    auto window = Transparent::RWindow{
        true,
        sf::VideoMode::getFullscreenModes().front(),
        "svetlichki", sf::Style::None
    };

    try
    {
        res::load(window.getSize());
    }
    catch(const std::exception& e)
    {
        MessageBoxA(window.getNativeHandle(), e.what(), "config :(", MB_OK | MB_ICONERROR);
    }
    if(res::textures.empty())
        return 1;

    window.setFramerateLimit(60);

    std::vector<Entity> entitys;
    for(auto &texture : res::textures)
        for(int i = 0; i < res::entitys_count; i++)
            entitys.emplace_back(texture);

    while(window.isOpen())
    {
        while(const auto event = window.pollEvent())
            if(event->is<sf::Event::Closed>())
                window.close();
        if(window.is_should_close())
            window.close();

        Entity::mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
        for(auto &e : entitys)
            e.update();

        window.trclear();
        for(auto &e : entitys)
            window.trdraw(e);
        window.trdisplay();
    }
    return 0;
}

int main()
{
    try
    {
        return run();
    } catch (std::exception &e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}