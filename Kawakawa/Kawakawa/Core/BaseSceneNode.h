#pragma once

#include "BaseSceneObject.h"
#include "../Math/KawaiiMath.h"

#include <string>
#include <vector>
#include <memory>

namespace Kawaii::Core
{
	/*
	* Base Scene Node 
	*/
	class BaseSceneNode
	{
	public:
		BaseSceneNode() = default;
		BaseSceneNode(std::string_view name) : m_Name(name) {}

		virtual ~BaseSceneNode() = default;

		void AppendChild(std::shared_ptr<BaseSceneNode>&& sub_node) { m_Chlidren.push_back(std::move(sub_node)); }

		const std::string& GetName() const { return m_Name; }
		// Get is the node updated
		bool Dirty() const { return m_Dirty; }
		void ClearDirty() { m_Dirty = false; }

	protected:
		std::string m_Name;
		std::list<std::shared_ptr<BaseSceneNode>> m_Chlidren;
		// TODO Transform
		mat4f m_RuntimeTransform = mat4f(1.0f);
		bool m_Dirty = true;
	};

	template<typename T>
	class SceneNode : public BaseSceneNode
	{
	public:
		using BaseSceneNode::BaseSceneNode;
		SceneNode() = default;

		void AddSceneObjectRef(std::weak_ptr<T> ref) { m_SceneObjectRef = ref; }
		std::weak_ptr<T> GetSceneObjectRef() { return m_SceneObjectRef; }

	protected:
		// Represents the mount point of the scene object on the node graph
		std::weak_ptr<T> m_SceneObjectRef;
	};

	// TODO
	// Specialize SceneNode template class for varius scene object type.
}