#include "EnginePCH.h"
#include "Common/World.h"
#include "Common/Entity.h"
#include "Graphics/Model.h"

namespace E3D
{	
	World::World()
	{
	}

	World::~World()
	{

	}

	void World::Tick(float deltaTime)
	{
		for (m_EntIter = m_Entities.begin(); m_EntIter != m_Entities.end();)
		{
			if ((*m_EntIter)->m_ShouldDelete)
			{
				delete *m_EntIter;
				m_EntIter = m_Entities.erase(m_EntIter);
			}
			else
			{
				(*m_EntIter)->Tick(deltaTime);
				m_EntIter++;
			}
		}
	}

	void World::Destroy()
	{
		for (m_EntIter = m_Entities.begin(); m_EntIter != m_Entities.end();)
		{
			(*m_EntIter)->Destroy();
			delete *m_EntIter;
			m_EntIter = m_Entities.erase(m_EntIter);
		}
	}


	void World::AddEntity(Entity* entity)
	{
		m_Entities.push_back(entity);
	}

	Entity* World::FindEntity(const std::string& name) 
	{
		for (m_EntIter = m_Entities.begin(); m_EntIter != m_Entities.end(); m_EntIter++)
		{
			if ((*m_EntIter)->GetName() == name)
			{
				return *m_EntIter;
			}
		}
		return nullptr;
	}
};