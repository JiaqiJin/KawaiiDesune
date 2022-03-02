#pragma once

glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 worldSide = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 worldForward = glm::vec3(0.f, 0.f, -1.f);

struct SpringData
{
	float stiffness;
	float restLength;

	SpringData(float stiffness, float restLength)
		: stiffness(stiffness), restLength(restLength) {}
};