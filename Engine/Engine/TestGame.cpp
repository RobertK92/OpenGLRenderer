#include "EnginePCH.h"
#include "TestGame.h"
#include "Graphics\PointLight.h"
#include "Graphics\DirectionalLight.h"
#include "Common\Input.h"
#include "Common\AssetLoader.h"
#include "Common\Config.h"
#include "Common\World.h"

#include "Common/Entity.h"
#include "Common/TestCom.h"

E3D_MAIN(MyGame::TestGame);

namespace MyGame 
{
	TestGame::TestGame()
	{
	}

	void TestGame::Initialise(int argc, char** argv)
	{
		m_Window->SetVsync(false);
		m_SleepTime = 0;
		
		Entity* camera = new Entity("Camera");
		camera->AddComponent<Camera>(new Camera());
		
		Entity* dirLight = new Entity("Directional Light");
		dirLight->AddComponent<DirectionalLight>(new DirectionalLight());

		Entity* test = new Entity("Test");
		test->AddComponent<TestCom>(new TestCom());
		test->AddComponent<Model>(new Model("Assets/Art/kat/Puss_in_Boots.obj"));
		test->SetPosition(0, 0, 0);
		
	}

	void TestGame::Tick(float deltaTime)
	{
		if (Input::GetKeyDown(Key::ESCAPE))
			Quit();

		m_World->Tick(deltaTime);
	}

	/*void TestGame::Initialise()
	{
		m_Window->SetVsync(false);
		m_SleepTime = 0;

		m_Camera = new Camera();	
		m_CameraPivot = new Transformable();
		m_Camera->SetParent(m_CameraPivot);

		Model* model = new Model("Assets/Art/TestScene.fbx");
		Model* sonic = new Model("Assets/Art/sonic/sonic.obj");
		sonic->SetScale(0.1f);
		sonic->SetPosition(0, -13, 0);
		//kat->Scale(1);
		//model->SetRotation(glm::radians(180.0f), glm::vec3(0, 1, 0));
		//model->SetRotation(glm::radians(-90.0f), glm::vec3(0, 1, 0));
		
		
		m_Models.push_back(model);
		m_Models.push_back(sonic);

		PointLight* pointLight = new PointLight();
		
		dirLight1 = new DirectionalLight(); 
		dirLight1->SetRotation(glm::vec3(-90, -90, 0));

		m_CameraPivot->SetPosition(glm::vec3(0, 10, 20));
		m_Camera->SetPosition(0, 0, 0);

		m_Models[0]->GetMesh("pCube1")->SetPosition(0, 0, -50);

		Input::HideCursor();
	}

	float cd = 0;
	void TestGame::Tick(float deltaTime)
	{
		if (Input::GetKeyDown(Key::ESCAPE))
			Quit();

		glm::vec2 center = glm::vec2(m_Window->GetWidth() * 0.5f, m_Window->GetHeight() * 0.5f);
		glm::vec2 mpos = Input::GetMousePosition();
		glm::vec2 motion = mpos - center;

		if (!m_Window->IsFocused())
			motion = glm::vec2();

		cd += deltaTime;
		if (Input::GetKeyDown(Key::B) && cd >= 1.0f)
		{
			m_Window->SetFullscreen(!m_Window->IsFullscreen());
			cd = 0.0f;
		}

		float speed = 10.0f * deltaTime;
		float rotSpeed = 0.1f;

		static Transformable* target2 = m_Models[1];
		static Transformable* target3 = m_Models[0]->GetMesh("pCone1");


		if (Input::GetKeyDown(Key::P))
			target2->SetParent(m_Camera);
		if (Input::GetKeyDown(Key::F))
			target2->UnParent();

		target3->Rotate(1.0f * deltaTime, glm::vec3(0, 1, 0));
		//target3->Rotate(glm::radians(ROT_SPEED), glm::vec3(1, 1, 0));

		m_Models[1]->Rotate(1.0f * deltaTime, glm::vec3(0, 1, 0));
		//m_Models[1]->Translate(m_Models[1]->GetForward() * speed);


		if (Input::GetKeyDown(Key::W))
			m_CameraPivot->Translate(m_Camera->GetForward() * speed);
		if (Input::GetKeyDown(Key::S))
			m_CameraPivot->Translate(-m_Camera->GetForward() * speed);
		if (Input::GetKeyDown(Key::D))
			m_CameraPivot->Translate(-m_Camera->GetRight() * speed);
		if (Input::GetKeyDown(Key::A))
			m_CameraPivot->Translate(m_Camera->GetRight() * speed);
		
	
		if (Input::GetMouseButtonDown(MouseButton::RIGHT)) 
		{
			glm::quat cRotY = glm::angleAxis<float>(glm::radians(motion.y) * rotSpeed, glm::vec3(1, 0, 0));
			glm::quat cRotX = glm::angleAxis<float>(glm::radians(motion.x) * rotSpeed, glm::vec3(0, 1, 0));

			m_CameraPivot->Rotate(cRotX);	// camerapivot yaw
			m_Camera->Rotate(cRotY);		// camera pitch

			Input::HideCursor();
			Input::CenterCursor();
		}
		else
		{
			Input::ShowCursor();
		}
	}*/

	void TestGame::ShutDown()
	{
	}

};

