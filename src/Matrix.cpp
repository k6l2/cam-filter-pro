#include "Matrix.h"
#include <cassert>
#include <queue>
#include <iostream>
#include <sstream>
const unsigned Matrix::CHARACTER_W = 16;
const unsigned Matrix::CHARACTER_H = 16;
bool Matrix::GlyphData::operator<(const GlyphData& other) const
{
    return intensity > other.intensity;
}
Matrix::Matrix(sf::RenderWindow& rw)
    :renderWindow(rw)
    ,theGrid()
    ,glyphCodex()
    ,glyphSprite()
    ,codexColumns(0)
    ,random()
    ,orderedGlyphIndices()
    ,orderedGlyphIndicesTex()
    ,minIntensity(0)
    ,maxIntensity(-1)
    ,m_shader()
    ,minMaxIntensity()
    ,minMaxShader()
    ,matrixIntensity()
    ,intensityShader()
    ,digitalRaindrops()
    ,digitalRaindropsPrev()
    ,digitalRaindropsShader()
    ,matrixRain()
    ,matrixRainPrev()
    ,matrixRainShader()
    ,texNoise()
    ,font()
    ,txtDisplay("", font)
    ,clock()
    ,avgFPS(60)
    ,seconds(0)
    ,debugMode(0)
    ,showFPS(false)
    ,m_appFocus(true)
{
    random.seed();
    if(!font.loadFromFile("assets/Tiny_04b.ttf"))
    {
        assert(false);
    }
    if(!glyphCodex.loadFromFile("assets/ascii.png"))
    {
        assert(false);
    }
    glyphSprite.setTexture(glyphCodex);
    glyphSprite.setTextureRect(sf::IntRect(0,0,CHARACTER_W,CHARACTER_H));
    codexColumns = glyphCodex.getSize().x/CHARACTER_W;
    unsigned codexRows = glyphCodex.getSize().y/CHARACTER_H;
    sf::Image glyphCodexData = glyphCodex.copyToImage();
    std::priority_queue<GlyphData> gDataQ;
    for(unsigned r = 0; r < codexRows; r++)
    {
        for(unsigned c = 0; c < codexColumns; c++)
        {
            unsigned long long glyphIntensity = 0;
            for(unsigned px = c*CHARACTER_W; px < c*CHARACTER_W + CHARACTER_W; px++)
            {
                for(unsigned py = r*CHARACTER_H; py < r*CHARACTER_H + CHARACTER_H; py++)
                {
                    sf::Color glyphPixel = glyphCodexData.getPixel(px, py);
                    unsigned pixelIntensity = (unsigned(glyphPixel.r) + unsigned(glyphPixel.g) + unsigned(glyphPixel.b))*unsigned(glyphPixel.a);
                    glyphIntensity += pixelIntensity;
                }
            }
            uint8_t glyphIndex = r*codexColumns + c;
            gDataQ.push({glyphIndex, glyphIntensity});
        }
    }
    // Initialize orderedGlyphIndicesTex //
    sf::Image orderedGlyphIndicesImage;
    orderedGlyphIndicesImage.create(16,16, sf::Color(0,0,0,0));
    while(!gDataQ.empty())
    {
        const GlyphData& leastIntense = gDataQ.top();
        unsigned texRow = orderedGlyphIndices.size()/16;
        unsigned texCol = orderedGlyphIndices.size()%16;
        orderedGlyphIndicesImage.setPixel(texCol, texRow, sf::Color(leastIntense.index,0,0,0));
        orderedGlyphIndices.push_back(leastIntense.index);
        gDataQ.pop();
    }
    orderedGlyphIndicesTex.loadFromImage(orderedGlyphIndicesImage);
    // Generate texNoise //
    texNoise.create(32, 32);
    texNoise.setRepeated(true);
    for(unsigned x = 0; x < texNoise.getSize().x; x++)
    {
        for(unsigned y = 0; y < texNoise.getSize().y; y++)
        {
            sf::RectangleShape rs({1,1});
            rs.setPosition(x,y);
            rs.setFillColor(sf::Color(random.range(0,255), random.range(0,255), random.range(0,255), random.range(0,255)));
            texNoise.draw(rs, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
        }
    }
    texNoise.display();
    // Initialize matrixIntensity //
    matrixIntensity.create(1920/CHARACTER_W + 1, 1080/CHARACTER_H + 1);
    if(!intensityShader.loadFromFile("assets/calculate-intensities.frag", sf::Shader::Fragment))
    {
        assert(false);
    }
    intensityShader.setParameter("characterSize", float(CHARACTER_W), float(CHARACTER_H));
    intensityShader.setParameter("matrixSize", float(matrixIntensity.getSize().x), float(matrixIntensity.getSize().y));
    // Initialize minMaxIntensity //
    minMaxIntensity.create(2,1);
    if(!minMaxShader.loadFromFile("assets/calculate-min-max-intensity.frag", sf::Shader::Fragment))
    {
        assert(false);
    }
    // Initialize digitalRaindrops //
    digitalRaindrops.create(matrixIntensity.getSize().x, 3);
    digitalRaindrops.clear(sf::Color(0,0,0,255));
    sf::RectangleShape rs({1, 2});
    for(unsigned x = 0; x < matrixIntensity.getSize().x; x++)
    {
        int randTime = random.range(0x100,0x2A00);
        rs.setPosition(x,0);
//        rs.setFillColor(sf::Color(0, 0, randTime&0xFF, (randTime>>8)&0xFF));//time & timer  (fixed float)
        rs.setFillColor(sf::Color(0, 0, 200, 0));//time & timer  (fixed float)
//        rs.setFillColor(sf::Color(0, 0, (x+1)%255, (x+1)%255));//time & timer  (fixed float)
        digitalRaindrops.draw(rs, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
    }
    digitalRaindrops.display();
    digitalRaindropsPrev.create(matrixIntensity.getSize().x, 3);
    if(!digitalRaindropsShader.loadFromFile("assets/process-digital-rain.frag", sf::Shader::Fragment))
    {
        assert(false);
    }
    digitalRaindropsShader.setParameter("digitalRaindropsPrev", digitalRaindropsPrev.getTexture());
    digitalRaindropsShader.setParameter("matrixIntensity", matrixIntensity.getTexture());
    digitalRaindropsShader.setParameter("noise", texNoise.getTexture());
    // Initialize matrixRain //
    matrixRain.create(matrixIntensity.getSize().x, matrixIntensity.getSize().y*3);
    matrixRain.clear(sf::Color(0,0,0,0));
    rs.setSize({float(matrixIntensity.getSize().x), float(matrixIntensity.getSize().y)});
    rs.setPosition(0,matrixIntensity.getSize().y*2);
    rs.setFillColor(sf::Color(255,255,255,255));
    matrixRain.draw(rs, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
    matrixRain.display();
    matrixRainPrev.create(matrixIntensity.getSize().x, matrixIntensity.getSize().y*3);
    if(!matrixRainShader.loadFromFile("assets/process-matrix-rain.frag", sf::Shader::Fragment))
    {
        assert(false);
    }
    matrixRainShader.setParameter("digitalRaindrops", digitalRaindrops.getTexture());
    matrixRainShader.setParameter("matrixIntensity", matrixIntensity.getTexture());
    matrixRainShader.setParameter("matrixRainPrev", matrixRainPrev.getTexture());
    // Initialize matrix shader //
    if(!m_shader.loadFromFile("assets/matrix.frag", sf::Shader::Fragment))
    {
        assert(false);
    }
    m_shader.setParameter("glyphCodex", glyphCodex);
    m_shader.setParameter("characterSize", float(CHARACTER_W), float(CHARACTER_H));
    m_shader.setParameter("codexColumns", float(codexColumns));
    m_shader.setParameter("orderedGlyphIndices", orderedGlyphIndicesTex);
    m_shader.setParameter("matrixIntensity", matrixIntensity.getTexture());
    m_shader.setParameter("minMaxIntensity", minMaxIntensity.getTexture());
    m_shader.setParameter("matrixRain", matrixRain.getTexture());
}
Matrix::~Matrix()
{
}
void Matrix::step(float deltaSeconds, const sf::Image& camImage)
{
    // (percentage of the screen that one character takes up) * (width of the camera) = pixels of the image get read by each character
    float pixelsPerCharacterW = float(CHARACTER_W)/renderWindow.getSize().x*camImage.getSize().x;
    float pixelsPerCharacterH = float(CHARACTER_H)/renderWindow.getSize().y*camImage.getSize().y;
    unsigned long long newMinIntensity = -1;
    unsigned long long newMaxIntensity = 0;
    unsigned long long minMaxRange = maxIntensity - minIntensity;
    for(unsigned r = 0; r < theGrid.size(); r++)
    {
        for(unsigned c = 0; c < theGrid[r].size(); c++)
        {
            //find the camera intensity for this character:
            theGrid[r][c].cameraIntensity = 0;
            for(unsigned x = c*pixelsPerCharacterW; x < c*pixelsPerCharacterW + pixelsPerCharacterW; x++)
            {
                for(unsigned y = r*pixelsPerCharacterH; y < r*pixelsPerCharacterH + pixelsPerCharacterH; y++)
                {
                    sf::Color cameraPixel = camImage.getPixel(std::min(x,camImage.getSize().x - 1),
                                                              std::min(y,camImage.getSize().y - 1));
                    unsigned pixelIntensity = unsigned(cameraPixel.r) + unsigned(cameraPixel.g) + unsigned(cameraPixel.b);
                    theGrid[r][c].cameraIntensity += pixelIntensity;
                }
            }
            if(theGrid[r][c].cameraIntensity < newMinIntensity)
            {
                newMinIntensity = theGrid[r][c].cameraIntensity;
            }
            if(theGrid[r][c].cameraIntensity > newMaxIntensity)
            {
                newMaxIntensity = theGrid[r][c].cameraIntensity;
            }
            if(minMaxRange <= 0)
            {
                theGrid[r][c].spriteId = orderedGlyphIndices[0];
            }
            else
            {
                unsigned index = float(theGrid[r][c].cameraIntensity - minIntensity)/minMaxRange*orderedGlyphIndices.size();
                index = std::min(std::max(index, unsigned(0)), orderedGlyphIndices.size() - 1);
                theGrid[r][c].spriteId = orderedGlyphIndices[index];
            }
            glyphSprite.setPosition(c*CHARACTER_W, r*CHARACTER_H);
            unsigned glyphRow = theGrid[r][c].spriteId/codexColumns;
            unsigned glyphCol = theGrid[r][c].spriteId%codexColumns;
            glyphSprite.setTextureRect(sf::IntRect(glyphCol*CHARACTER_W,glyphRow*CHARACTER_H,
                                                   CHARACTER_W,CHARACTER_H));
            renderWindow.draw(glyphSprite);
        }
    }
    minIntensity = newMinIntensity;
    maxIntensity = newMaxIntensity;
}
void Matrix::stepShader(float deltaSeconds, const sf::Sprite& camSprite)
{
	if (debugMode == 5)
	{
		renderWindow.draw(camSprite);
	}
	else
	{
		// calculate fps //
		sf::Time timePassedSinceLastFrame = clock.getElapsedTime();
		clock.restart();
		float fps = 1/(timePassedSinceLastFrame.asSeconds() > 0 ? timePassedSinceLastFrame.asSeconds() : 1/16.f);
		avgFPS = 0.9*avgFPS + 0.1*fps;
		seconds += timePassedSinceLastFrame.asSeconds();
		// process digital raindrops //
		sf::Sprite sprDigitalRaindrops(digitalRaindrops.getTexture());
		digitalRaindropsPrev.draw(sprDigitalRaindrops, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
		digitalRaindropsPrev.display();
		digitalRaindropsShader.setParameter("deltaSeconds", timePassedSinceLastFrame.asSeconds());
		digitalRaindropsShader.setParameter("time", seconds);
		sf::RenderStates statesDigitalRain(&digitalRaindropsShader);
		statesDigitalRain.blendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero);
		digitalRaindrops.draw(sprDigitalRaindrops, statesDigitalRain);
		digitalRaindrops.display();
		// process matrix rain //
		sf::Sprite sprMatrixRain(matrixRain.getTexture());
		matrixRainPrev.draw(sprMatrixRain, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
		matrixRainPrev.display();
		matrixRainShader.setParameter("deltaSeconds", timePassedSinceLastFrame.asSeconds());
		sf::RenderStates statesMatrixRain(&matrixRainShader);
		statesMatrixRain.blendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero);
		matrixRain.draw(sprMatrixRain, statesMatrixRain);
		matrixRain.display();
		// build intensity matrix //
		matrixIntensity.draw(camSprite, &intensityShader);
		matrixIntensity.display();
		// calculate min/max intensity //
		sf::Sprite sprIntensity(matrixIntensity.getTexture());
		minMaxIntensity.draw(sprIntensity, &minMaxShader);
		minMaxIntensity.display();
		// draw The Matrix //
		switch (debugMode)
		{
		case 0:
			renderWindow.draw(camSprite, &m_shader);
			break;
		case 1:
			sprIntensity.setScale(2.5, 2.5);
			renderWindow.draw(sprIntensity);
			break;
		case 2:
		{
			sf::Sprite sprMinMax(minMaxIntensity.getTexture());
			sprMinMax.setScale(100, 100);
			renderWindow.draw(sprMinMax);
		}
		break;
		case 3:
			sprDigitalRaindrops.setScale(7, 100);
			renderWindow.draw(sprDigitalRaindrops);
			break;
		case 4:
			sprMatrixRain.setScale(2.5, 2.5);
			renderWindow.draw(sprMatrixRain);
			break;
		}
	}
    // draw FPS //
    if(showFPS)
    {
        std::stringstream ss;
        ss << "FPS:" << int(avgFPS);
        txtDisplay.setString(ss.str());
        txtDisplay.setPosition(renderWindow.getView().getSize().x - txtDisplay.getLocalBounds().width, 0);
        sf::RectangleShape rs({txtDisplay.getGlobalBounds().width, txtDisplay.getGlobalBounds().height*2});
        rs.setPosition(renderWindow.getView().getSize().x - txtDisplay.getLocalBounds().width, 0);
        rs.setFillColor(sf::Color::Black);
        renderWindow.draw(rs);
        renderWindow.draw(txtDisplay);
    }
    /// DEBUG ///
    if(m_appFocus)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
        {
            debugMode = 0;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
        {
            debugMode = 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
        {
            debugMode = 2;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
        {
            debugMode = 3;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
        {
            debugMode = 4;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
        {
            debugMode = 5;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F7))
        {
            showFPS = true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F8))
        {
            showFPS = false;
        }
    }
//    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
//    {
//        sf::Vector2i pos = sf::Mouse::getPosition(renderWindow);
//        sf::Image img = digitalRaindrops.getTexture().copyToImage();
//        sf::Vector2i imgPos(pos.x/7, pos.y/100);
//        if(imgPos.x < img.getSize().x && imgPos.y < img.getSize().y)
//        {
//            sf::Color pixel = img.getPixel(imgPos.x, imgPos.y);
//            std::cout<<int(pixel.r)<<","<<int(pixel.g)<<","<<int(pixel.b)<<","<<int(pixel.a)<<"\n";
//        }
//    }
}
void Matrix::onResize()
{
    std::cout<<"renderwindow.size=("<<renderWindow.getSize().x<<","<<renderWindow.getSize().y<<")\n";
    sf::Vector2u gridSize(std::max(renderWindow.getSize().x/CHARACTER_W, unsigned(1)) + 1,
                          std::max(renderWindow.getSize().y/CHARACTER_H, unsigned(1)) + 1);
    std::cout<<"gridSize=("<<gridSize.x<<","<<gridSize.y<<")\n";
    unsigned curr = 0;
    theGrid.resize(gridSize.y);
    for(unsigned r = 0; r < theGrid.size(); r++)
    {
        theGrid[r].resize(gridSize.x);
        for(unsigned c = 0; c < theGrid[r].size(); c++)
        {
//            theGrid[r][c].spriteId = abs(random.rInt())%256;
            theGrid[r][c].spriteId = orderedGlyphIndices[(curr++)%orderedGlyphIndices.size()];
        }
    }
}
void Matrix::appFocus(bool val)
{
    m_appFocus = val;
}
