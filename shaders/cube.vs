#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 uv;

out vec3 worldPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 invModel;

void main()
{
	worldPos = vec3(model * vec4(aPos, 1.0));
	Normal = vec3(vec4(aNormal, 0.0) * invModel);
	TexCoords = uv;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}