#pragma once

#include "Common/Game.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include "Common/Transformable.h"
#include "Common/World.h"

namespace MyGame 
{
	using namespace E3D;
	class TestGame : public Game
	{
	public:
		TestGame();
		virtual void Initialise(int argc, char** argv) override;
		virtual void Tick(float deltaTime) override;
		virtual void ShutDown() override;

	};
};