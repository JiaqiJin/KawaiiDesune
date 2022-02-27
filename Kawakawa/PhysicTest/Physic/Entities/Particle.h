#pragma once

#include "Body.h"

namespace Physic
{
	class Particle : public Body
	{
	public:
		Particle(const glm::vec3& position = glm::vec3(), const float inverseMass = 0);
		~Particle();

		virtual void Integrate(float deltaTime) override;

		virtual void ClearAccumulators() override;
	};
}