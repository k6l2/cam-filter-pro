#ifndef APPLICATION_H
#define APPLICATION_H
#include <SFML/Graphics.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Matrix.h"
#define USE_OPENCV
class Application
{
    public:
        Application(sf::RenderWindow& rw);
        virtual ~Application();
        bool step(float deltaSeconds);
        void onResize();
        void appFocus(bool val);
    private:
        std::string propertyToFourCC(double property);
    private:
#ifdef USE_OPENCV
        cv::VideoCapture camera;
#endif
        sf::RenderWindow& renderWindow;
        sf::Image cameraImage;
        sf::Texture cameraTex;
        sf::Sprite cameraSprite;
        Matrix theMatrix;
        bool m_appFocus;
};

#endif // APPLICATION_H
