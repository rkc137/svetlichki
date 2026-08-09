#include <list>

#include "winhandle.hpp"
#include "Entity.hpp"

int main();
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) { return main(); }

int run()
{
    FreeConsole();
    sf::RenderWindow window(
        sf::VideoMode::getFullscreenModes().front(),
        "svetlichki", sf::Style::None
    );

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
    auto winhandle = Transparent::Winhandle{window, res::is_rgba_important};
    sf::RenderTexture render_texture{sf::VideoMode::getFullscreenModes().front().size};

    std::vector<Entity> entitys;
    for(auto &texture : res::textures)
        for(int i = 0; i < res::entitys_count; i++)
            entitys.emplace_back(texture);

    while(window.isOpen())
    {
        while(const auto event = window.pollEvent())
            if(event->is<sf::Event::Closed>())
                window.close();
        if(winhandle.is_should_close())
            window.close();

        Entity::mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());

        if(res::is_rgba_important)
        {
            render_texture.clear(sf::Color::Transparent);
            for(auto &e : entitys)
            {
                e.update();
                render_texture.draw(e);
            }
            render_texture.display();
            winhandle.update_render(render_texture);
        }
        else
        {
            window.clear(sf::Color::Black);
            for(auto &e : entitys)
            {
                e.update();
                window.draw(e);
            }
            window.display();
        }
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