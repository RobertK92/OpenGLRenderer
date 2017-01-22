#pragma once

#include "Common/Transformable.h"

namespace E3D
{
	class Component;

	class Entity final : public Transformable
	{
		friend class World;
	public:
		Entity(const std::string& name = "Nameless Entity");
		~Entity();

		template<class T> T* AddComponent(T* component)
		{
			if (!component)
			{
				E3DDebugPrint("Failed to add Component -> Component is nullptr");
				return nullptr;
			}

			for (m_ComIter = m_Components.begin(); m_ComIter != m_Components.end(); m_ComIter++)
			{
				if (dynamic_cast<T*>(*m_ComIter) != nullptr)
				{
					E3DDebugPrint("Failed to add Component -> Component of type '" << typeid(T).name() << "' already present on Entity");
					return nullptr;
				}
			}

			component->m_Entity = this;
			m_Components.push_back(component);

			component->Initialise();
			return component;
		}

		template<class T> T* GetComponent()
		{
			for (m_ComIter = m_Components.begin(); m_ComIter != m_Components.end(); m_ComIter++)
			{
				if (dynamic_cast<T*>(*m_ComIter) != nullptr)
				{
					return *m_ComIter;
				}
			}
			return nullptr;
		}

		template<class T> void RemoveComponent()
		{
			for (m_ComIter = m_Components.begin(); m_ComIter != m_Components.end(); m_ComIter++)
			{
				if (dynamic_cast<T*>(*m_ComIter) != nullptr)
				{
					delete *m_ComIter;
					m_ComIter = m_Components.erase(m_ComIter);
					break;
				}
			}
		}

		virtual void OnTransformed() override;

		void Destroy();

		//////////////////////////////////////////////////////////////////////////
		// Getters
		//////////////////////////////////////////////////////////////////////////
		const std::string& GetName() const;

		//////////////////////////////////////////////////////////////////////////
		// Setters
		//////////////////////////////////////////////////////////////////////////
		void SetName(const std::string& name); // !

		//////////////////////////////////////////////////////////////////////////

	private:
		void Tick(float deltaTime);
		
		bool m_ShouldDelete;
		std::string m_Name;
		std::vector<Component*>::iterator m_ComIter;
		std::vector<Component*> m_Components;
	};
};