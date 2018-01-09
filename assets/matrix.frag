#version 130
uniform sampler2D texture;
uniform sampler2D glyphCodex;
uniform vec2 characterSize;
uniform float codexColumns;
uniform sampler2D orderedGlyphIndices;
uniform sampler2D matrixIntensity;
uniform sampler2D minMaxIntensity;
uniform sampler2D matrixRain;
float texelToIntensity(vec4 texel)
{
	return texel.r*255 + texel.g*255 + texel.b*255 + texel.a*255;
}
float texelToFloat(vec4 texel)
{
	float wholeNumber = texel.r*255 + texel.g*(255.0*pow(2.0,8.0));
	float fraction = (texel.b*255 + texel.a*(255.0*pow(2.0,8.0)))/(pow(2.0,16.0) - 1);
	return wholeNumber + fraction;
}
void main()
{
	float minIntensity = texelToIntensity(texture2D(minMaxIntensity, vec2(0.25,0.5)));//pixel(0,0)
	float maxIntensity = texelToIntensity(texture2D(minMaxIntensity, vec2(0.75,0.5)));//pixel(1,0);
	vec4 texel;
	//Fist, need to find which matrix character index we are out
	ivec2 cameraTexSize = textureSize(texture, 0);
	ivec2 cameraTexFragPosition = ivec2(cameraTexSize*gl_TexCoord[0].st);
	ivec2 matrixIndex = ivec2(cameraTexFragPosition/characterSize);
	//next, find the intensity of "texture" in the space this character occupies
	vec2 matrixSize = textureSize(matrixIntensity, 0);
	vec2 matrixST = matrixIndex/matrixSize;
	matrixST = vec2(matrixST.s, 1.0 - matrixST.t);
	float intensity = texelToIntensity(texture2D(matrixIntensity, matrixST));
	//then, determine which index of orderedGlyphIndices to select [0,255]
	int orderedIndex = int((intensity - minIntensity)/(maxIntensity - minIntensity)*256);
	vec2 orderedGlyphIndicesCoord = vec2(orderedIndex%16, orderedIndex/16)/textureSize(orderedGlyphIndices, 0);
	int spriteId = int(texture2D(orderedGlyphIndices, orderedGlyphIndicesCoord).r*256);
	//using the ordered glyph index, find the st coordinates of the glyphCodex we are going to use
	ivec2 texStart = ivec2(matrixIndex*characterSize);
	vec2 characterDelta = (cameraTexFragPosition - texStart)/characterSize;
	ivec2 glyphIndex = ivec2(spriteId%int(codexColumns), spriteId/codexColumns);
	ivec2 glyphPosition = ivec2(glyphIndex*characterSize);
	vec2 glyphTexCoord = (glyphPosition + characterSize*characterDelta)/textureSize(glyphCodex, 0);
	//finally, set the texel to the glyphCodex fragment we need to use
	texel = texture2D(glyphCodex, glyphTexCoord);
	//modify the alpha of texel based on matrixRain
	float lifeTime = texelToFloat(texture2D(matrixRain, vec2(matrixST.s, matrixST.t/3)));
	float lifeTimer = texelToFloat(texture2D(matrixRain, vec2(matrixST.s, matrixST.t/3 + 1.0/3)));
	texel.a *= lifeTimer/lifeTime;
	//Color
	//modify the intensity based on matrixRain
	const float ACTIVATION_TIME = 0.25;
	const float MAX_INTENSITY = 16*16*3;
	float activationTimer = texelToFloat(texture2D(matrixRain, vec2(matrixST.s, matrixST.t/3 + 2.0/3)));
	float activationProgress = activationTimer/ACTIVATION_TIME;
	intensity = intensity + (MAX_INTENSITY - intensity)*activationProgress;
	float colorComponent = (intensity - minIntensity)/(maxIntensity - minIntensity);
	texel.r = texel.b = colorComponent;
    gl_FragColor = gl_Color*texel;
}