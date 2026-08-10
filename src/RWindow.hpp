#pragma once

#include <SFML/Graphics.hpp>

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

namespace Transparent
{

class RWindow : public sf::RenderWindow
{
public:
    template <typename... Args>
    RWindow(bool is_LWA, Args&&... args)
        : sf::RenderWindow(std::forward<Args>(args)...)
    { setup_transparent(is_LWA); }

    RWindow(RWindow&& other) = delete;
    RWindow(const RWindow&) = delete;
    RWindow& operator=(RWindow&& other) = delete;
    RWindow& operator=(const RWindow&) = delete;
    ~RWindow();

    void update_render(sf::RenderTexture& rt);
    bool is_should_close();
    void trclear();
    void trdisplay();
    void trdraw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

private:
    void setup_transparent(bool is_LWA);

    sf::RenderTexture render_texture;
    NOTIFYICONDATA nid = {};
    bool is_LWA_ALPHA = true;
    bool is_closing = false;
    static constexpr unsigned IDI_ICON1 = 1;
};

}