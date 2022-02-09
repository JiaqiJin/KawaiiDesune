#pragma once

#include "../../Interface/IModule.h"
#include "../../Core/Memory/MemoryManager.h"
#include "Component/TransformComponent.h"
#include "Component/MeshRenderComponent.h"
#include "Component/CameraComponent.h"

#include <crossguid/guid.hpp>

namespace Excalibur
{
	class World;
	class Entity : public IModule
	{
	public:
		Entity();
		Entity(const xg::Guid& guid);
		virtual ~Entity();

		virtual int	Initialize() noexcept;
		virtual int	Initialize(World* world) noexcept;
		virtual void Finalize() noexcept;

		xg::Guid GetGuid() const;
		void	SetGuid(const xg::Guid& guid);

		void AddChild(std::shared_ptr<Entity> child);
		void RemoveChild(std::shared_ptr<Entity> child);
		Entity* GetParent();
		void SetParent(Entity* parent);
		bool IsChild(std::shared_ptr<Entity> child);
		size_t GetChildrenCount();

		World* GetWorld() { return m_World; }

		template<typename T>T* AddComponent();
		template<typename T>T* GetComponent();
		template<typename T>void RemoveComponent();

	protected:
		xg::Guid m_Guid;
		Entity* m_Parent;
		World* m_World;
		std::vector<std::shared_ptr<Entity>> m_Children;

		// Components
		TransformComponent* m_Transform;
		CameraComponent* m_Camera;
		MeshRenderComponent* m_MeshRender;
	};


	template<typename T>
	T* Entity::AddComponent()
	{
		void* Comp;
		if (std::is_same<T, TransformComponent>::value)
		{
			m_Transform = new TransformComponent();
			m_Transform->SetMaster(this);
			m_Transform->Initialize();
			Comp = m_Transform;
		}

		if (std::is_same<T, MeshRenderComponent>::value) 
		{
			m_MeshRender = new MeshRenderComponent();
			m_MeshRender->SetMaster(this);
			m_MeshRender->Initialize();
			Comp = m_MeshRender;
		}

		if (std::is_same<T, CameraComponent>::value)
		{
			m_Camera = new CameraComponent();
			m_Camera->SetMaster(this);
			m_Camera->Initialize();
			Comp = m_Camera;
		}
		return (T*)Comp;
	}

	template<typename T>
	T* Entity::GetComponent()
	{
		void* ret = nullptr;
		if (std::is_same<T, TransformComponent>::value) {
			ret = m_Transform;
		}
		else if (std::is_same<T, MeshRenderComponent>::value) {
			ret = m_MeshRender;
		}
		else if (std::is_same<T, CameraComponent>::value) {
			ret = m_Camera;
		}
		return (T*)ret;
	}

	template<typename T>
	void Entity::RemoveComponent()
	{
		if (std::is_same<T, TransformComponent>::value) {
			delete m_Transform;
			m_Transform = nullptr;
		}
		if (std::is_same<T, MeshRenderComponent>::value) {
			delete m_MeshRender;
			m_MeshRender = nullptr;
		}
		if (std::is_same<T, CameraComponent>::value) {
			delete m_Camera;
			m_Camera = nullptr;
		}
	}
}