#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>
#include "Toolkit.hpp"



class Game
{
    AdjustableView view;

    sf::Vector2f mousePos(sf::RenderWindow& screen)
    {
        sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(screen));
        return mouse;
    }

    sf::Vector2f windowSize(sf::RenderWindow& screen)
    {
        sf::Vector2f winSize = sf::Vector2f(screen.getSize());
        return winSize;
    }
public:
    void main(sf::RenderWindow& window)
    {
        view.setSize(windowSize(window));

        float time = 0;
        float time_limit = 1;

        sf::Image sheet;
        sheet.loadFromFile("sheet.png");

        PVector tileSize(10, 10);
        TileSet tiles(sheet, tileSize);

        std::cout << tiles.size();

        sf::Clock clock;
        while (window.isOpen())
        {
            float dt = clock.restart().asSeconds();

            time += dt;
            if(time >= time_limit)
            {
                time = 0;
            }

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
            }
            window.clear();


            window.display();
        }
    }
};

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");
    Game().main(window);
    return 0;
}
