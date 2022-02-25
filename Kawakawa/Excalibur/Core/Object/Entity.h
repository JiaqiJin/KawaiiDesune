#pragma once
#include <vector>

#include "../../Interface/IModule.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRenderComponent.h"
#include "Components/CameraComponent.h"

#include <crossguid/guid.hpp>

namespace Excalibur
{
	class World;

	class Entity : public IModule 
	{
	public:
		virtual int	Initialize();
		virtual int	Initialize(World* world);
		virtual void Finalize();
		
	public:
		Entity();
		Entity(const xg::Guid& guid);
		virtual ~Entity();
		xg::Guid GetGuid() const;
		void SetGuid(const xg::Guid& guid);

		void	AddChild(std::shared_ptr<Entity> child);
		void	RemoveChild(std::shared_ptr<Entity> child);
		Entity* GetParent();
		void	SetParent(Entity* parent);
		bool	IsChild(std::shared_ptr<Entity> child);
		size_t	GetChildrenCount();

		World*	GetWorld() { return m_World; }

		template<typename T>T*	AddComponent();
		template<typename T>T*	GetComponent();
		template<typename T>void RemoveComponent();
		

	protected:
		xg::Guid mGuid;
		Entity*	m_Parent;
		World*	m_World;
		std::vector<std::shared_ptr<Entity>> m_Children;

	protected:
		TransformComponent*	m_Transform;
		MeshRenderComponent* m_MeshRender;
		CameraComponent* m_Camera;

	};

	template<typename T>
	T*	Excalibur::Entity::AddComponent() 
	{
		void* comp;
		if (std::is_same<T, TransformComponent>::value) 
		{
			m_Transform = new TransformComponent();
			m_Transform->SetMaster(this);
			m_Transform->Initialize();
			comp = m_Transform;
		}

		if (std::is_same<T, MeshRenderComponent>::value) 
		{
			m_MeshRender = new MeshRenderComponent();
			m_MeshRender->SetMaster(this);
			m_MeshRender->Initialize();
			comp = m_MeshRender;
		}

		if (std::is_same<T, CameraComponent>::value) 
		{
			m_Camera = new CameraComponent();
			m_Camera->SetMaster(this);
			m_Camera->Initialize();
			comp = m_Camera;
		}
		return (T*)comp;
	}

	template<typename T>
	T*		Excalibur::Entity::GetComponent()
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
		else if (std::is_same<T, CameraComponent>::value) 
		{
			ret = m_Camera;
		}
		return (T*)ret;
	}

	template<typename T>
	void	Excalibur::Entity::RemoveComponent()
	{
		if (std::is_same<T, TransformComponent>::value) 
		{
			delete m_Transform;
			m_Transform = nullptr;
		}
		if (std::is_same<T, MeshRenderComponent>::value)
		{
			delete m_MeshRender;
			m_MeshRender = nullptr;
		}
		if (std::is_same<T, CameraComponent>::value) 
		{
			delete m_Camera;
			m_Camera = nullptr;
		}
	}
}
