#pragma once

#include "../Math/KawaiiMath.h"
#include "../Core/Buffer.h"
#include "../AssertManager/Image.h"

#include <crossguid/guid.hpp>
#include <filesystem>

#include "../Utility/SceneObjectTypeDef.h"

/*
* GUID : Resource Manager System store diff version of resources and also stores the relationships between resources.
* There needs to be a mapping relationship between this external system and the scene objects inside our program.
*/

namespace Kawaii::Core
{
	class BaseSceneObject
	{
	protected:
		BaseSceneObject(SceneObjectType type);
		virtual ~BaseSceneObject() = default;

		BaseSceneObject() = default;
		BaseSceneObject(const BaseSceneObject & obj) = default;
		BaseSceneObject& operator=(const BaseSceneObject & obj) = default;
	public:
		const xg::Guid& GetGuid() const { return m_Guid; }
		const SceneObjectType GetType() const { return m_Type; }
	protected:
		// Global Unique ID
		const xg::Guid m_Guid;
		const SceneObjectType m_Type;
	};
}