#version 440

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 FragColor;

// Terrain texture
uniform sampler2D terrainTexture;

void main()
{
     FragColor = texture(terrainTexture, vs_texcoord);
	 //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // set all 4 vector values to 1.0 so it is pure white
}