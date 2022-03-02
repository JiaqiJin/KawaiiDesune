#include "../PrecompiledHeader.h"
#include "Body.h"

namespace Physic
{
	Body::Body() { }

	Body::~Body() { }

	void Body::SetMass(const float mass)
	{
		if (mass == 0)
			inverseMass = 0;
		else
			inverseMass = 1.f / mass;
	}

	float Body::GetMass() const
	{
		if (inverseMass == 0)
			return MAX_MASS;
		else
			return 1.f / inverseMass;
	}
}