#version 440

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform samplerCube skybox2;
uniform float blendFactor;

void main()
{    
	vec4 day = texture(skybox, TexCoords);
	vec4 night = texture(skybox2, TexCoords);
	vec4 finalColour = mix(day, night, blendFactor);

	FragColor = finalColour;
  //FragColor = vec4(0.5, 0.0, 0.0, 1.0);
}