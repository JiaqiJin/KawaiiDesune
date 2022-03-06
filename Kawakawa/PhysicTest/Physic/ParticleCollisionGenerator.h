#pragma once

namespace Physic
{
	class ParticleCollisionGenerator
	{
	public:
		virtual void GenerateContact() = 0;
	};
}