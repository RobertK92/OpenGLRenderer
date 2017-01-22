#include "EnginePCH.h"
#include "Common/TestCom.h"

namespace E3D
{

	void TestCom::Initialise()
	{
	}

	void TestCom::Tick(float deltaTime)
	{
		m_Entity->Translate(deltaTime, 0, 0);
	}
};