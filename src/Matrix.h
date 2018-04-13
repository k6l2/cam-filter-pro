#ifndef MATRIX_H
#define MATRIX_H
#include <SFML/Graphics.hpp>
#include "Random.h"
class Matrix
{
    public:
        static const unsigned CHARACTER_W;
        static const unsigned CHARACTER_H;
        struct GridElement
        {
            uint8_t spriteId;// index in glyphCodex
            unsigned long long cameraIntensity;
        };
    private:
		static const char* const FONT_DATA;
		static const char* const GLYPH_CODEX;
        struct GlyphData
        {
            uint8_t index;
            unsigned long long intensity;
            bool operator<(const GlyphData& other) const;
        };
    public:
        Matrix(sf::RenderWindow& rw);
        ~Matrix();
        void step(float deltaSeconds, const sf::Image& camImage);
        void stepShader(float deltaSeconds, const sf::Sprite& camSprite);
        void onResize();
        void appFocus(bool val);
    private:
        sf::RenderWindow& renderWindow;
        std::vector<std::vector<GridElement> > theGrid;
        sf::Texture glyphCodex;
        sf::Sprite glyphSprite;
        unsigned codexColumns;
        Random random;
        std::vector<uint8_t> orderedGlyphIndices;//ordered from least to most intense
        sf::Texture orderedGlyphIndicesTex;//for the shader
        unsigned long long minIntensity;
        unsigned long long maxIntensity;
        sf::Shader m_shader;
        sf::RenderTexture minMaxIntensity;//(0,0) pixel = min, (1,0) pixel = max
        sf::Shader minMaxShader;
        sf::RenderTexture matrixIntensity;
        sf::Shader intensityShader;
        sf::RenderTexture digitalRaindrops;
        sf::RenderTexture digitalRaindropsPrev;
        sf::Shader digitalRaindropsShader;
        sf::RenderTexture matrixRain;
        sf::RenderTexture matrixRainPrev;
        sf::Shader matrixRainShader;
        sf::RenderTexture texNoise;
        sf::Font font;
        sf::Text txtDisplay;
        sf::Clock clock;
        float avgFPS;
        float seconds;
        unsigned debugMode;
        bool showFPS;
        bool m_appFocus;
};

#endif // MATRIX_H
