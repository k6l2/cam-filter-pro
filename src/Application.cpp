#include "Application.h"
#include <iostream>
#include "escapi.h"
Application::Application(sf::RenderWindow& rw)
    :
#ifdef USE_OPENCV
    camera(0),
#endif
    renderWindow(rw)
    ,cameraImage()
    ,cameraTex()
    ,cameraSprite()
    ,theMatrix(rw)
    ,m_appFocus(true)
{
#ifdef USE_OPENCV
    if(!camera.isOpened())
    {
        std::cerr<<"ERROR: camera didn't open!";
        assert(false);
    }
//    camera.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));
    camera.set(CV_CAP_PROP_FPS, 30);
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    std::cout<<"fps="<<camera.get(CV_CAP_PROP_FPS)<<"\n";
//    std::cout<<"hue="<<camera.get(CV_CAP_PROP_HUE)<<" saturation="<<camera.get(CV_CAP_PROP_SATURATION)<<"\n";
//    std::cout<<"camera FourCC=\""<<propertyToFourCC(camera.get(CV_CAP_PROP_FOURCC))<<"\"\n";
//    camera.set(CV_CAP_PROP_HUE, 0.f);
//    camera.set(CV_CAP_PROP_SATURATION, 128.f);
//    camera.set(CV_CAP_PROP_GAMMA, 0.f);
//    std::cout<<"hue="<<camera.get(CV_CAP_PROP_HUE)<<" saturation="<<camera.get(CV_CAP_PROP_SATURATION)<<"\n";
    sf::Vector2u cameraSize(unsigned(camera.get(CV_CAP_PROP_FRAME_WIDTH)),
                            unsigned(camera.get(CV_CAP_PROP_FRAME_HEIGHT)));
    std::cout<<"cameraSize=("<<cameraSize.x<<","<<cameraSize.y<<")\n";
    cameraImage.create(cameraSize.x, cameraSize.y, sf::Color(0,255,0));
#else
    int devices = setupESCAPI();
    if(devices == 0)
    {
        assert(false);
    }
    std::cout<<"devices="<<devices<<std::endl;
    SimpleCapParams captureParams;
    captureParams.mWidth = 1920;
    captureParams.mHeight = 1080;
    cameraImage.create(captureParams.mWidth, captureParams.mHeight, sf::Color(0,255,0));
    captureParams.mTargetBuf =  (int*)cameraImage.getPixelsPtr();
    if(initCapture(0, &captureParams) == 0)
    {
        assert(false);
    }
#endif
    if(!cameraTex.loadFromImage(cameraImage))
    {
        std::cerr<<"ERROR: couldn't load texture??!";
        assert(false);
    }
    cameraSprite.setTexture(cameraTex, true);
    cameraSprite.setPosition(0, 0);
    renderWindow.setSize({renderWindow.getSize().x+1,renderWindow.getSize().y});
}
Application::~Application()
{
}
bool Application::step(float deltaSeconds)
{
    if(m_appFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        return false;
    }
#ifdef USE_OPENCV
//    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
//    {
//        sf::Vector2i pos = sf::Mouse::getPosition();
//        sf::Vector2f posNorm(float(pos.x)/renderWindow.getSize().x,
//                             float(pos.y)/renderWindow.getSize().y);
//        camera.set(CV_CAP_PROP_BRIGHTNESS, 255*posNorm.y);
////        camera.set(CV_CAP_PROP_SATURATION, 255*posNorm.y);
////        std::cout<<"hue="<<camera.get(CV_CAP_PROP_HUE)<<" saturation="<<camera.get(CV_CAP_PROP_SATURATION)<<"\n";
//    }
    cv::Mat frame;
    bool success = camera.read(frame);
    if(!success)
    {
        std::cerr<<"ERROR: couldn't read camera frame!";
        return false;
    }
//    std::cout<<"rows,cols=("<<frame.rows<<","<<frame.cols<<")\n";
//    std::cout<<"hue="<<camera.get(CV_CAP_PROP_HUE)<<" saturation="<<camera.get(CV_CAP_PROP_SATURATION)<<"\n";
    for(int r = 0; r < frame.rows; r++)
    {
        for(int c = 0; c < frame.cols; c++)
        {
            const cv::Vec3b& framePixel = frame.at<cv::Vec3b>(r,c);
            cameraImage.setPixel(c,r, sf::Color(framePixel[0],framePixel[1],framePixel[2]));
        }
    }
    cameraTex.update(cameraImage);
#endif
    cameraSprite.setScale(float(renderWindow.getSize().x)/cameraTex.getSize().x,
                          float(renderWindow.getSize().y)/cameraTex.getSize().y);
    sf::View view = renderWindow.getView();
    view.setSize(renderWindow.getSize().x, renderWindow.getSize().y);
    view.setCenter(renderWindow.getSize().x/2, renderWindow.getSize().y/2);
    renderWindow.setView(view);
    theMatrix.stepShader(deltaSeconds, cameraSprite);
//    theMatrix.step(deltaSeconds, cameraImage);
    return true;
}
std::string Application::propertyToFourCC(double property)
{
    int iProp = static_cast<int>(property);
    char fourCC[5];
    for(int i = 0; i < 4; i++)
    {
        fourCC[i] = iProp >> (i * 8);
    }
    fourCC[4] = '\0';
    return std::string(fourCC);
}
void Application::onResize()
{
    theMatrix.onResize();
}
void Application::appFocus(bool val)
{
    m_appFocus = val;
    theMatrix.appFocus(val);
}
