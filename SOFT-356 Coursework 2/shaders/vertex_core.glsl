#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
	vs_normal = mat3(ModelMatrix) * vertex_normal;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f); 
	// SHOULDNT DO THIS IN THE SHADER, SVEN SAID THAT IT SLOWS IT DOWN
	// NEED TO DO WHAT HE DID IN THE LECTURE AND THEN PASS IT IN ONCE YOU HAVE CALCULATED IT
}