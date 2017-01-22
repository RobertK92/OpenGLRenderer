#include "EnginePCH.h"
#include "Common/Component.h"

namespace E3D
{
	void Component::Initialise() { }
	void Component::Tick(float deltaTime) { }
	void Component::OnTransformed() { }

	Entity* Component::GetEntity() const
	{
		return m_Entity;
	}

	Component::Component()
	{

	}

	Component::~Component()
	{

	}

};