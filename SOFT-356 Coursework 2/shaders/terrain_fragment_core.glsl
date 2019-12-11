#version 440

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

// Terrain texture
uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;

out vec4 FragColor;

void main()
{
	vec4 blendMapColour = texture(blendMap, vs_texcoord);

	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	vec2 tiledCoords = vs_texcoord  * 40.0;
	vec4 backgroundTextureColour = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 rTextureColour = texture(rTexture, tiledCoords) * blendMapColour.r;
	vec4 gTextureColour = texture(gTexture, tiledCoords) * blendMapColour.g;
	vec4 bTextureColour = texture(bTexture, tiledCoords) * blendMapColour.b;

	vec4 totalColour =backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;

     FragColor = totalColour;
	 //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // set all 4 vector values to 1.0 so it is pure white
}