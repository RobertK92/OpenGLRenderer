#pragma once

#include "Common/Component.h"

namespace E3D
{
	class TestCom : public Component
	{
	public:
		virtual void Initialise() override;
		virtual void Tick(float deltaTime) override;

	protected:
		
	};	
};