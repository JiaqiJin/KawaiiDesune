#include <type_traits>
#include <iostream>
#include "Entity.h"

namespace Excalibur
{
	int Entity::Initialize()
	{
		return 0;
	}

	int Entity::Initialize(Excalibur::World* world) 
	{
		m_World = world;

		AddComponent<TransformComponent>();

		return 0;
	}

	void Entity::Finalize()
	{
		m_Children.clear();
		m_Parent = nullptr;
	}

	Entity::Entity() :
		m_MeshRender(nullptr),
		m_Camera(nullptr),
		m_Transform(nullptr)
	{
		mGuid = xg::newGuid();
	}

	Entity::Entity(const xg::Guid& guid) :
		m_Parent(nullptr),
		m_Transform(nullptr),
		mGuid(guid)
	{

	}

	Entity::~Entity()
	{
		std::cout << "destructor of entity: " << mGuid << std::endl;
	}

	xg::Guid Entity::GetGuid() const
	{
		return mGuid;
	}

	void Entity::SetGuid(const xg::Guid& guid)
	{
		mGuid = guid;
	}

	void Excalibur::Entity::AddChild(std::shared_ptr<Entity> child)
	{
		if (IsChild(child)) {
			return;
		}
		m_Children.push_back(child);
		child->SetParent(this);
	}

	void Entity::RemoveChild(std::shared_ptr<Entity> child) 
	{
		if (!IsChild(child)) {
			return;
		}
		auto iter = m_Children.begin();
		for (; iter != m_Children.end(); ++iter) {
			if ((*iter) == child) {
				m_Children.erase(iter);
				child->SetParent(nullptr);
				return;
			}
		}
	}

	bool Entity::IsChild(std::shared_ptr<Entity> child)
	{
		if (child->GetParent() == this) {
			return true;
		}
		return false;
	}

	Entity* Entity::GetParent()
	{
		return m_Parent;
	}

	void Entity::SetParent(Excalibur::Entity* parent) 
	{
		m_Parent = parent;
	}

	size_t Entity::GetChildrenCount()
	{
		return m_Children.size();
	}
}