#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float opacity;
	float shininess;
	sampler2D diffuseTex;
	sampler2D specularTex;
	sampler2D reflectionTex;
	sampler2D bumpTex;

	int isDiffuseTex;
	int isSpecularTex;
	int isBumpTex;
};


in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

//Uniforms
uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// Calculates the ambient value
vec3 calculateAmbient(Material material)
{
	return lightAmbient * material.ambient;
}

// Calculates the diffuse value
vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
	vec3 norm = normalize(vs_normal);
	vec3 lightDir = normalize(lightPos0 - vs_position);
	float diffuse = max(dot(norm, lightDir), 0.5);
	vec3 diffuseFinal = lightDiffuse * (material.diffuse * diffuse);

	return diffuseFinal;
}

// Calculates the specular value
vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 viewDir = normalize(cameraPos - vs_position);
	vec3 lightDir = normalize(lightPos0 - vs_position);
	vec3 reflectDir = reflect(-lightDir, normalize(vs_normal));
	float specularConstant = pow(max(dot(viewDir, reflectDir), 0.0), 35);
	
	// If there is a specular texture then use it
	if (material.isSpecularTex == 1) {
		vec3 specularFinal =  lightSpecular * (specularConstant * material.specular * texture(material.specularTex, vs_texcoord).rgb);
		return specularFinal;
	} else {
		vec3 specularFinal =  lightSpecular * specularConstant;
		return specularFinal;
	}

}



void main()
{

//Ambient light
vec3 ambientFinal = calculateAmbient(material);

//Diffuse light
vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, lightPos0);

//Specular light
vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, lightPos0, cameraPos);

// If there is a diffuse material then use it, if not then use the colours instead
if (material.isDiffuseTex == 1) 
{
	fs_color = 
	texture(material.diffuseTex, vs_texcoord)
	* (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
} else 
{
	vec3 result = material.diffuse * (ambientFinal + diffuseFinal + specularFinal); // <--- this seems to work
	//vec3 result = ambientFinal + diffuseFinal + specularFinal; <!--- This is what the tutorials uses however the one above seems to solve the issue with low poly boat not colouring correctly!
	fs_color = vec4(result, material.opacity); // The last property is opacity (RGBA)
}

}
