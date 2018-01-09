#include <SFML/Graphics.hpp>
#include "Application.h"
int main(int argc, char** argv)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Matrix Webcam Filter");
    window.setFramerateLimit(60);
    sf::Event event;
    Application app(window);
    bool resizeCorrected = false;
    while (window.isOpen())
    {
        resizeCorrected = false;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                if(!resizeCorrected)
                {
                    const float TARGET_RATIO = 1920/1080.f;
                    if(fabsf(float(window.getSize().x)/window.getSize().y - TARGET_RATIO) > 0.00001)
                    {
                        window.setSize({unsigned(window.getSize().y*TARGET_RATIO), window.getSize().y});
                        app.onResize();
                    }
                }
                resizeCorrected = true;
                break;
            case sf::Event::LostFocus:
                app.appFocus(false);
                break;
            case sf::Event::GainedFocus:
                app.appFocus(true);
                break;
            }
        }
        window.clear({0,0,0,255});
        if(!app.step(1/60.f))
        {
            break;
        }
        window.display();
    }
    return EXIT_SUCCESS;
}
