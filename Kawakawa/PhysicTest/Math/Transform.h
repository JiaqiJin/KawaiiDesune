#pragma once

#include "../PrecompiledHeader.h"
#include "../Common.h"

namespace Physic
{
	static glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
	static glm::vec3 worldSide = glm::vec3(1.f, 0.f, 0.f);
	static glm::vec3 worldForward = glm::vec3(0.f, 0.f, -1.f);

	class Transform
	{
	public:
		Transform(const glm::vec3& position = glm::vec3(), 
			const glm::quat& orientation = glm::angleAxis(0.f, worldUp), const glm::vec3& scale = glm::vec3(1, 1, 1)) :
			position(position), orientation(orientation), scale(scale)
		{}

		~Transform() = default;

		inline void Translate(const glm::vec3 & translationVec) { position += translationVec; }
		inline void Rotate(const glm::quat & rotation) { orientation = rotation * orientation; }

		inline void SetPosition(const glm::vec3& pos) { position = pos; }
		inline void SetOrientation(const glm::quat& orientation) { this->orientation = orientation; }
		inline void SetScale(const glm::vec3& scale) { this->scale = scale; }

		inline const glm::vec3& GetPosition() const { return position; }
		inline const glm::quat& GetOrientation() const { return orientation; }
		inline const glm::vec3& GetScale() const { return scale; }

		inline glm::vec3& GetPosition() { return position; }
		inline glm::quat& GetOrientation() { return orientation; }
		inline glm::vec3& GetScale() { return scale; }
	private:
		glm::vec3 position;
		glm::quat orientation;
		glm::vec3 scale;
	};
}