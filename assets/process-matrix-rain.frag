#version 130
// Matrix Rain Properties
	// ActivationTimer
		//Animation from white -> normal color
		//fixed float
	// ActivationTime
		//Could just be a CONST float?
	// LifeTimer
		//fixed float
	// LifeTime
		//fixed float
uniform sampler2D texture;
uniform sampler2D matrixRainPrev;
uniform sampler2D digitalRaindrops;
uniform sampler2D matrixIntensity;
uniform float deltaSeconds;
int texelToInt(vec4 texel)
{
	int rVal = int(texel.r*255.0);
	int gVal = int(texel.g*255.0*pow(2.0,8.0));
	int bVal = int(texel.b*255.0*pow(2.0,16.0));
	// int aVal = int(texel.a*(255.0*float(pow(2.0,24.0))));
	int aVal = 0;// For the sake of debugging, I am going to leave the integral values at 24-bit so I can see them when drawn
	return rVal + gVal + bVal + aVal;
}
vec4 intToTexel(int i)
{
	return vec4((i&255)/255.0, ((i>>8)&255)/255.0, ((i>>16)&255)/255.0, 1.0);
}
float texelToFloat(vec4 texel)
{
	float wholeNumber = texel.r*255 + texel.g*(255.0*pow(2.0,8.0));
	float fraction = (texel.b*255 + texel.a*(255.0*pow(2.0,8.0)))/(pow(2.0,16.0) - 1);
	return wholeNumber + fraction;
}
vec4 floatToTexel(float f)
{
	int wholeNumber = int(floor(f));
	int fixedFraction = int(fract(f)*(pow(2.0,16.0) - 1));
	return vec4((wholeNumber&255)/255.0, ((wholeNumber>>8)&255)/255.0,
				(fixedFraction&255)/255.0, ((fixedFraction>>8)&255)/255.0);
}
vec4 updateActivationTimer(float prevActivationTimer, int rainDropRow, int row)
{
	const float ACTIVATION_TIME = 0.25;
	if(row == rainDropRow)
	{
		// return vec4(0,1,0,0.5);
		prevActivationTimer = ACTIVATION_TIME;
	}
	else
	{
		// return vec4(0,0,0,0.5);
		prevActivationTimer -= deltaSeconds;
	}
	if(prevActivationTimer < 0.0)
	{
		prevActivationTimer = 0.0;
	}
	return floatToTexel(prevActivationTimer);
}
vec4 updateLifeTimer(float prevLifeTimer, float prevActivationTimer, int rainDropRow, int row)
{
	const float LIFE_TIME = 5.999;///TODO: make this a variable in digitalRaindrops
	if(row == rainDropRow)
	{
		prevLifeTimer = LIFE_TIME;
	}
	else if(prevActivationTimer == 0.0)
	{
		prevLifeTimer -= deltaSeconds;
	}
	if(prevLifeTimer < 0.0)
	{
		prevLifeTimer = 0.0;
	}
	return floatToTexel(prevLifeTimer);
}
vec4 updateLifeTime(float prevLifeTime, int rainDropRow, int row)
{
	const float LIFE_TIME = 5.999;///TODO: make this a variable in digitalRaindrops
	if(row == rainDropRow)
	{
		prevLifeTime = LIFE_TIME;
	}
	return floatToTexel(prevLifeTime);
}
void main()
{
	ivec2 texSize = textureSize(texture, 0);
	ivec2 matrixSize = textureSize(matrixIntensity, 0);
	// int offsetY = int(gl_FragCoord.y)%(matrixSize.y); //For some reason this isn't working????
	float offsetY;
	if(gl_FragCoord.y < matrixSize.y)
	{
		offsetY = gl_FragCoord.y;
	}
	else if(gl_FragCoord.y < matrixSize.y*2)
	{
		offsetY = gl_FragCoord.y - matrixSize.y;
	}
	else
	{
		offsetY = gl_FragCoord.y - matrixSize.y*2;
	}
	vec4 prevTexelLifeTime = texture2D(matrixRainPrev, vec2(gl_FragCoord.x, offsetY)/vec2(texSize));
	vec4 prevTexelLifeTimer = texture2D(matrixRainPrev, vec2(gl_FragCoord.x, matrixSize.y + offsetY)/vec2(texSize));
	vec4 prevTexelActivationTimer = texture2D(matrixRainPrev, vec2(gl_FragCoord.x, matrixSize.y*2 + offsetY)/vec2(texSize));
	vec4 texelRainDropRow = texture2D(digitalRaindrops, vec2(gl_FragCoord.x, 0.5)/vec2(textureSize(digitalRaindrops, 0)));
	vec4 texel;
	int propertyIndex = int(gl_FragCoord.y)/matrixSize.y;
	switch(propertyIndex)
	{
	case 0:
		texel = updateLifeTime(texelToFloat(prevTexelLifeTime),
							   matrixSize.y - texelToInt(texelRainDropRow), int(offsetY));
		// texel = prevTexelLifeTime;
		break;
	case 1:
		texel = updateLifeTimer(texelToFloat(prevTexelLifeTimer), texelToFloat(prevTexelActivationTimer),
								matrixSize.y - texelToInt(texelRainDropRow), int(offsetY));
		// texel = prevTexelLifeTimer;
		// texel = vec4(1,0,0,0.5);
		break;
	case 2:
		texel = updateActivationTimer(texelToFloat(prevTexelActivationTimer),
									  matrixSize.y - texelToInt(texelRainDropRow), int(offsetY));
		// texel = prevTexelActivationTimer;
		break;
	}
	gl_FragColor = texel;
}