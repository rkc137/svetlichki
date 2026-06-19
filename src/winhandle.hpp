#pragma once

#include <SFML/Graphics.hpp>

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

class Winhandle
{
public:
    Winhandle(sf::RenderWindow &window, bool is_LWA_ALPHA);
    ~Winhandle();

    Winhandle() = delete;
    Winhandle(const Winhandle&) = delete;
    Winhandle& operator=(const Winhandle&) = delete;
    Winhandle(Winhandle&&) = delete;
    Winhandle& operator=(Winhandle&&) = delete;

    void update_render(sf::RenderTexture& rt);
    bool is_should_close();

private:
    NOTIFYICONDATA nid = {};
    bool is_LWA_ALPHA = true;
    bool is_closing = false;
    sf::RenderWindow &window;
};