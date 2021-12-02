#pragma once

#include "../../Interface/IModule.h"
#include "../../Core/Memory/MemoryManager.h"
#include "Component/TransformComponent.h"
#include "Component/MeshRenderComponent.h"

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
		xg::Guid GetGuid() const noexcept { return m_Guid; }
		void SetGuid(const  xg::Guid& guid) noexcept { m_Guid = guid; }

		// Virtual Functions
		virtual int		Initialize() noexcept;
		virtual int		Initialize(World* world) noexcept;
		virtual void	Finalize() noexcept;

		void	AddChild(std::shared_ptr<Entity> child);
		void	RemoveChild(std::shared_ptr<Entity> child);
		Entity* GetParent();
		void	SetParent(Entity* parent);
		bool	IsChild(std::shared_ptr<Entity> child);
		size_t	GetChildrenCount();

		World* GetWorld() { return m_World; }

		template<typename T> T* AddComponent();
		template<typename T> T* GetComponent();
		template<typename T> void RemoveComponent();
	private:
		xg::Guid m_Guid;
		Entity* m_Parent;
		World* m_World;
		std::vector<std::shared_ptr<Entity>> m_Children;

	protected:
		TransformComponent* m_Transform;
		MeshRenderComponent* m_MeshRender;
	};

	template<typename T>
	T* Entity::AddComponent()
	{
		auto MemoryManager = GMemoryManager::GetInstance();
		void* comp = nullptr;
		if (std::is_same<T, TransformComponent>::value)
		{
			m_Transform = new TransformComponent();
			m_Transform->SetMaster(this);
			m_Transform->Initialize();
			comp = m_Transform;
		}

		else if (std::is_same<T, MeshRenderComponent>::value)
		{
			m_MeshRender = new MeshRenderComponent();
			m_MeshRender->SetMaster(this);
			m_MeshRender->Initialize();
			comp = m_MeshRender;
		}

		return (T*)comp;
	}

	template<typename T>
	T*  Entity::GetComponent()
	{
		void* ret = nullptr;
		if (std::is_same<T, TransformComponent>::value)
		{
			ret = m_Transform;
		}
		else if (std::is_same<T, MeshRenderComponent>::value) 
		{
			ret = m_MeshRender;
		}
	}

	template<typename T>
	void Entity::RemoveComponent()
	{
		if (std::is_same<T, TransformComponent>::value) 
		{
			m_Transform->Finalize();
			delete m_Transform;
			m_Transform = nullptr;
		}
		else if (std::is_same<T, MeshRenderComponent>::value)
		{
			m_MeshRender->Finalize();
			m_MeshRender = nullptr;
		}
	}
}