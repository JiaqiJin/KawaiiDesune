#pragma once

namespace Physic
{
	class IIntegratable
	{
	public:
		// Call a physics update
		virtual void Integrate(float deltaTime) = 0;	
	};
}