#pragma once

#include <SFML/Graphics.hpp>

class Winhandle
{
public:
    Winhandle(sf::RenderWindow &window);
    ~Winhandle();

    Winhandle() = delete;
    Winhandle(const Winhandle&) = delete;
    Winhandle& operator=(const Winhandle&) = delete;
    Winhandle(Winhandle&&) = delete;
    Winhandle& operator=(Winhandle&&) = delete;

    bool is_should_close;
private:
    struct Wdata;
    std::unique_ptr<Wdata> wdata;
    sf::RenderWindow &window;
};