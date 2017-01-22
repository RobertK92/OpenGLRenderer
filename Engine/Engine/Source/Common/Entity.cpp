#include "EnginePCH.h"
#include "Common/Entity.h"
#include "Common/Component.h"
#include "Common/Game.h"

namespace E3D
{
	void Entity::Tick(float deltaTime)
	{
		for (m_ComIter = m_Components.begin(); m_ComIter != m_Components.end(); m_ComIter++)
		{
			(*m_ComIter)->Tick(deltaTime);
		}
	}

	Entity::Entity(const std::string& name)
	{
		m_ShouldDelete = false;
		SetName(name);
		Game::Ptr()->GetActiveWorld()->AddEntity(this);
	}

	Entity::~Entity()
	{	
	}

	void Entity::Destroy()
	{
		// Call this function on my children
		for (m_ChildIter = m_Children.begin(); m_ChildIter != m_Children.end(); m_ChildIter++)
		{
			dynamic_cast<Entity*>(*m_ChildIter)->Destroy();
		}

		// Destroy and erase Components on this Entity
		for (m_ComIter = m_Components.begin(); m_ComIter != m_Components.end();)
		{
			delete *m_ComIter;
			m_ComIter = m_Components.erase(m_ComIter);
		}

		m_ShouldDelete = true;
	}

	const std::string& Entity::GetName() const
	{
		return m_Name;
	}

	// !
	void Entity::SetName(const std::string& name)
	{
		m_Name = name;
	}

	void Entity::OnTransformed()
	{
		for (m_ComIter = m_Components.begin(); m_ComIter != m_Components.end(); m_ComIter++)
			(*m_ComIter)->OnTransformed();
	}

};