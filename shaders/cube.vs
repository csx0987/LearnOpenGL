#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 worldPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 invModel;

void main()
{
	worldPos = vec3(model * vec4(aPos, 1.0));
	normal = vec3(vec4(aNormal, 0.0) * invModel);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}