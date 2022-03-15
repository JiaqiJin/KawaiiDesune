#pragma once

namespace Physic
{
	struct SpringData
	{
		float stiffness;
		float restLength;

		SpringData(float stiffness, float restLength)
			: stiffness(stiffness), restLength(restLength) {}
	};
}