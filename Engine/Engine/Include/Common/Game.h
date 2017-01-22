#pragma once

#include "Graphics/RenderWindow.h"
#include "Common/World.h"

namespace E3D
{
	class World;

	class Game
	{
	public:
		static Game* Ptr()
		{
			return m_Instance;
		}

		virtual void Initialise(int argc, char** argv);
		virtual void Tick(float deltaTime);
		virtual void ShutDown();

		RenderWindow* GetWindow() const;
		World* GetActiveWorld() const;
		bool IsRunning();
		void Quit();
		

		void __InternalTick();
		void __InternalInit(Game* game);

	protected:
		Game();
		virtual ~Game();

		RenderWindow* m_Window;
		World* m_World;
		uint m_SleepTime;

	private:
		static Game* m_Instance;

		float m_LastTime;
		float m_DeltaTime;
		bool m_IsRunning;
		bool m_FirstRun;
	};
};