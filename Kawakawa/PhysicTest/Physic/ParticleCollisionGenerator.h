#pragma once

namespace Physic
{
	class ParticleCollisionGenerator
	{
	public:
		virtual void GenerateContact(std::list<ParticleCollision>& collisions, unsigned int limit) = 0;
	};
}