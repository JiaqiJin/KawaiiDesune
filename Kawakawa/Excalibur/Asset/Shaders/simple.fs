#version 410

in vec3 Normal;
in vec3 WPosition; 
in vec4 Color;

out vec4 FragColor;

void main ()
{
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}