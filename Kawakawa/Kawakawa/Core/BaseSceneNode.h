#pragma once

#include "BaseSceneObject.h"
#include "Common/SceneObject.h"
#include "../Math/KawaiiMath.h"

#include <string>
#include <vector>
#include <memory>

namespace Kawaii::Core
{
	/*
	* Base Scene Node 
	* This base class is actually a node of a standard tree structure.
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
		std::string                                      m_Name;
		std::list<std::shared_ptr<BaseSceneNode>>        m_Chlidren;
		std::list<std::shared_ptr<SceneObjectTransform>> m_Transforms;
		mat4f                                            m_RuntimeTransform = mat4f(1.0f);
		bool                                             m_Dirty = true;
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

	using SceneEmptyNode = BaseSceneNode;
	class SceneGeometryNode : public SceneNode<SceneObjectGeometry>
	{
	public:
		using SceneNode::SceneNode;

		void       SetVisibility(bool visible) { m_Visible = visible; }
		void       SetIfCastShadow(bool shadow) { m_Shadow = shadow; }
		void       SetIfMotionBlur(bool motion_blur) { m_MotionBlur = motion_blur; }
		const bool Visible() { return m_Visible; }
		const bool CastShadow() { return m_Shadow; }
		const bool MotionBlur() { return m_MotionBlur; }

		using SceneNode::AddSceneObjectRef;
	protected:

		bool m_Visible = true;
		bool m_Shadow = true;
		bool m_MotionBlur = false;
	};

	// TODO
	// Specialize SceneNode template class for varius scene object type.
}