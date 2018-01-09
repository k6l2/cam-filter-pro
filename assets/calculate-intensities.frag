uniform sampler2D texture;// this should be the camera texture
uniform vec2 characterSize;
uniform vec2 matrixSize;
float calculateTextureIntensity(ivec2 matrixIndex)
{
	ivec2 cameraTexSize = textureSize(texture, 0);
	ivec2 texStart = matrixIndex*ivec2(characterSize);
	float intensity = 0.0;
	for(int r = texStart.y; r < texStart.y + int(characterSize.y); r++)
	{
		for(int c = texStart.x; c < texStart.x + int(characterSize.x); c++)
		{
			vec4 texel = texture2D(texture, vec2(c,r)/vec2(cameraTexSize));
			intensity += texel.r + texel.g + texel.b;
		}
	}
	return intensity;
}
vec4 intensityToTexel(float intensity)
{
	vec4 retVal = vec4(0,0,0,1);
	if(intensity > 0.0)
	{
		if(intensity > 255.0)
		{
			retVal.r = 1.0;
		}
		else
		{
			retVal.r = intensity/255.0;
		}
		intensity -= 255.0;
	}
	if(intensity > 0.0)
	{
		if(intensity > 255.0)
		{
			retVal.g = 1.0;
		}
		else
		{
			retVal.g = intensity/255.0;
		}
		intensity -= 255.0;
	}
	if(intensity > 0.0)
	{
		if(intensity > 255.0)
		{
			retVal.b = 1.0;
		}
		else
		{
			retVal.b = intensity/255.0;
		}
		intensity -= 255.0;
	}
	if(intensity > 0.0)
	{
		if(intensity > 255.0)
		{
			retVal.a = 1.0;
		}
		else
		{
			retVal.a = intensity/255.0;
		}
		intensity -= 255.0;
	}
	return retVal;
}
void main()
{
	ivec2 matrixIndex = ivec2(gl_FragCoord.x, matrixSize.y - gl_FragCoord.y);
	float intensity = calculateTextureIntensity(matrixIndex);
	gl_FragColor = intensityToTexel(intensity);
}