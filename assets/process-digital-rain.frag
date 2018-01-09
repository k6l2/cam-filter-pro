#version 130
// Digital Raindrop Properties
	// Column
		//implicit
	// Row
		//some ridiculously huge # that will never loop.. I guess Uint32
	// TimeUntilNextRow
		//float
	// TimeUntilNextRowTimer
		//float
uniform sampler2D texture;
uniform sampler2D digitalRaindropsPrev;
uniform sampler2D matrixIntensity;
uniform sampler2D noise;
uniform float deltaSeconds;
uniform float time;
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
vec4 updateRow(int prevRow, float prevTimer)
{
	ivec2 matrixSize = textureSize(matrixIntensity, 0);
	if(prevTimer == 0)
	{
		prevRow++;
	}
	if(prevRow >= matrixSize.y)
	{
		prevRow = 0;
	}
	return intToTexel(prevRow);
}
vec4 updateNextRowTimer(float prevTimer, float prevTime)
{
	if(prevTimer <= 0)
	{
		prevTimer = prevTime - deltaSeconds;
	}
	else
	{
		prevTimer -= deltaSeconds;
	}
	//everything is fucked if these numbers go negative, so let's not do that...
	if(prevTimer < 0)
	{
		prevTimer = 0;
	}
	return floatToTexel(prevTimer);
}
float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
vec4 updateNextRowTime(float prevTime, int prevRow)
{
	ivec2 matrixSize = textureSize(matrixIntensity, 0);
	if(prevRow >= matrixSize.y - 1)
	{
		const float MIN_TIME = 0.0039063096055543;
		const float MAX_TIME = 0.1640650034332799;
		vec4 randomTexel = texture2D(noise, rand(gl_FragCoord.xy) + vec2(time, time));
		prevTime = MIN_TIME + (MAX_TIME - MIN_TIME)*randomTexel.r;
	}
	return floatToTexel(prevTime);
}
void main()
{
	ivec2 texSize = textureSize(texture, 0);
	vec4 prevTexelRow = texture2D(digitalRaindropsPrev, vec2(gl_FragCoord.x, 0.5)/vec2(texSize));
	vec4 prevTexelNextRowTimer = texture2D(digitalRaindropsPrev, vec2(gl_FragCoord.x, 1.5)/vec2(texSize));
	vec4 prevTexelNextRowTime = texture2D(digitalRaindropsPrev, vec2(gl_FragCoord.x, 2.5)/vec2(texSize));
	vec4 texel;
	switch(int(floor(gl_FragCoord.y)))
	{
	case 0:
		texel = updateRow(texelToInt(prevTexelRow), texelToFloat(prevTexelNextRowTimer));
		// prevTexelRow.r += 1.0*deltaSeconds;
		// if(prevTexelRow.r > 1.0)
		// {
			// prevTexelRow.r -= 1.0;
		// }
		// texel = prevTexelRow;
		break;
	case 1:
		texel = updateNextRowTimer(texelToFloat(prevTexelNextRowTimer), texelToFloat(prevTexelNextRowTime));
		// texel = prevTexelNextRowTimer;
		break;
	case 2:
		texel = updateNextRowTime(texelToFloat(prevTexelNextRowTime), texelToInt(prevTexelRow));
		// texel = prevTexelNextRowTime;
		break;
	}
	gl_FragColor = texel;
}