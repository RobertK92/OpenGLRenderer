#include "EnginePCH.h"
#include "Common/Game.h"
#include "Common/Input.h"
#include "Common/Config.h"

namespace E3D
{
	Game* Game::m_Instance;

	Game::Game()
	{
		Config::Init();
		
		m_Window = new RenderWindow(Config::DefaultScreenWidth(), Config::DefaultScreenHeight(), E3D_STRINGIFY(E3D));
		Input::Init(m_Window);

		m_DeltaTime = (float)glfwGetTime();
		m_IsRunning = true;
		m_SleepTime = 2;
		m_FirstRun = true;
		m_World = new World();
	}

	Game::~Game() 
	{
		m_Window->Destroy();
		delete m_Window;
	}

	RenderWindow* Game::GetWindow() const
	{
		return m_Window;
	}

	World* Game::GetActiveWorld() const
	{
		return m_World;
	}

	bool Game::IsRunning()
	{
		return m_IsRunning && !glfwWindowShouldClose(m_Window->m_Window);
	}

	void Game::Quit()
	{
		m_IsRunning = false;
	}

	void Game::__InternalInit(Game* game)
	{
		m_Instance = game;
	}

	void Game::__InternalTick()
	{
#ifdef _WIN32
		Sleep(m_SleepTime);
#endif // _WIN32

		m_Window->Clear();
		float current = (float)glfwGetTime();
		m_DeltaTime = current - m_LastTime;
		m_LastTime = current;
		if(!m_FirstRun)
			Tick(m_DeltaTime);
		m_Window->Swap();
		Input::Reset();
		m_FirstRun = false;
	}


	void Game::Initialise(int argc, char** argv) { }
	void Game::Tick(float deltaTime) { }
	void Game::ShutDown() { }
};