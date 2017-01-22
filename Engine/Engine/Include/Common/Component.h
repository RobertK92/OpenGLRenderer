#pragma once

#include "Common/Entity.h"

namespace E3D
{
	class Component
	{
		friend class Entity;
	public:
		Component();
		virtual ~Component();

		virtual void Initialise();
		virtual void Tick(float deltaTime);
		virtual void OnTransformed();

		Entity* GetEntity() const;

	protected:
		
		Entity* m_Entity;
	};
};