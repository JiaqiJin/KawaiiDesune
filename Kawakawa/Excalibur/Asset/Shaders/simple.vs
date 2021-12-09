#version 410

layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;

out vec3 Normal;
out vec3 WPosition;
out vec4 Color;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main () {
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition, 1.0f);
	WPosition = vec3( worldMatrix * vec4(inputPosition, 1.0f));
	Normal = mat3(transpose(inverse(worldMatrix))) * inputNormal;
	Color = vec4(0.9f, 0.3f, 0.5f, 1.0f);
}