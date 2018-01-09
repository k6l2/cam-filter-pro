uniform sampler2D texture;
//OUTPUT: pixel(0,0)=min | pixel(1,0)=max
void main()
{
	ivec2 matrixGridSize = textureSize(texture, 0);
	bool searchMin = gl_FragCoord.x < 1;
	float closestValue;
	vec4 closestTexel = vec4(0,0,0,1.0);
	if(searchMin)
	{
		closestValue = 4.0;
	}
	else
	{
		closestValue = 0;
	}
	for(int x = 0; x < matrixGridSize.x; x++)
	{
		for(int y = 0; y < matrixGridSize.y; y++)
		{
			// float intensity = calculateTextureIntensity(ivec2(x,y));
			vec4 intensityTexel = texture2D(texture, vec2(x,y));
			float intensity = intensityTexel.r + intensityTexel.g + intensityTexel.b + intensityTexel.a;
			if(searchMin)
			{
				if(intensity < closestValue)
				{
					closestValue = intensity;
					closestTexel = intensityTexel;
				}
			}
			else
			{
				if(intensity > closestValue)
				{
					closestValue = intensity;
					closestTexel = intensityTexel;
				}
			}
		}
	}
	gl_FragColor = closestTexel;
}