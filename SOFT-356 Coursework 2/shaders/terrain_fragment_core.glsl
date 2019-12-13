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

uniform vec3 lightPos0;
uniform vec3 cameraPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;


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

    // FragColor = totalColour;
	 //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // set all 4 vector values to 1.0 so it is pure white

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightAmbient;    
    
     // diffuse 
    vec3 norm = normalize(vs_normal);
    vec3 lightDir = normalize(lightPos0 - vs_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightDiffuse;
    
    // specular
    //float specularStrength = 0.5;
   // vec3 viewDir = normalize(-vs_position); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
   // vec3 reflectDir = reflect(-lightDir, norm);  
  //  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  //  vec3 specular = specularStrength * spec * lightSpecular; 
    
    //vec4 result = vec3((ambient + diffuse + specular)) * totalColour;
	vec3  result = ambient + diffuse + 0;
	vec3 test = vec3(totalColour);
    vec3  resultWithTex = (ambient + diffuse + 0) * test;
	FragColor = vec4(resultWithTex, 1.0);
}