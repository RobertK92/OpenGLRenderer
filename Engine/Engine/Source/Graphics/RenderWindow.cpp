#include "EnginePCH.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/MeshBatch.h"
#include "Graphics/Camera.h"
#include "Common/Config.h"
#include "Common/Input.h"

namespace E3D 
{
	//////////////////////////////////////////////////////////////////////////
	// Glfw Callbacks
	//////////////////////////////////////////////////////////////////////////
	void RenderWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Input::m_MouseWheel.x = (float)xoffset;
		Input::m_MouseWheel.y = (float)yoffset;
	}

	void RenderWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		Renderer::GetInstance()->GetWindow()->m_Width = width;
		Renderer::GetInstance()->GetWindow()->m_Height = height;
	}

	void RenderWindow::WindowFocusCallback(GLFWwindow* window, int focused)
	{
		if(focused == GL_TRUE)
			Renderer::GetInstance()->GetWindow()->m_Focused = true;
		else 
			Renderer::GetInstance()->GetWindow()->m_Focused = false;
	}

	//////////////////////////////////////////////////////////////////////////

	RenderWindow::RenderWindow(int width, int height, char* title, bool vsync)
	{
		m_Width = width;
		m_Height = height;
		m_Title = title;
		m_Vsync = vsync;

		m_WindowedWidth = m_Width;
		m_WindowedHeight = m_Height;
		m_ScreenMode = ScreenMode::Windowed;
		m_Focused = true;
		m_FirstRun = true;

		m_Renderer = Renderer::GetInstance();
		m_Renderer->SetWindow(this);

		if (!glfwInit())
			E3DShowError("Failed to initialize glfw");

		CreateRenderWindow(m_ScreenMode);
	}

	void RenderWindow::CreateRenderWindow(const ScreenMode& screenMode)
	{
		m_ScreenMode = screenMode;
		glfwWindowHint(GLFW_SAMPLES, Config::AASamples());
		glfwWindowHint(GLFW_RESIZABLE, false);

		switch (m_ScreenMode)
		{
		case ScreenMode::Fullscreen:
		{
			const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			m_Width = mode->width;
			m_Height = mode->height;

			m_Fullscreen = true;
			if (m_Window) 
			{
				GLFWwindow* newWindow = glfwCreateWindow(m_Width, m_Height, m_Title, glfwGetPrimaryMonitor(), m_Window);
				glfwDestroyWindow(m_Window);
				m_Window = newWindow;
			} 
			else
			{
				m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, glfwGetPrimaryMonitor(), NULL);
			}
			
			break;
		}
		case ScreenMode::FullscreenWindowed:
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode * mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			m_Width = mode->width;
			m_Height = mode->height;

			m_Fullscreen = false;
			if (m_Window)
			{
				GLFWwindow* newWindow = glfwCreateWindow(m_Width, m_Height, m_Title, monitor, m_Window);
				glfwDestroyWindow(m_Window);
				m_Window = newWindow;
			}
			else
			{
				m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, monitor, NULL);
			}
			break;
		}
		case ScreenMode::Windowed:
		{
			m_Fullscreen = false;
			if (m_Window)
			{
				GLFWwindow* newWindow = glfwCreateWindow(m_WindowedWidth, m_WindowedHeight, m_Title, NULL, m_Window);
				glfwDestroyWindow(m_Window);
				m_Window = newWindow;
				m_Width = m_WindowedWidth;
				m_Height = m_WindowedHeight;
			}
			else
			{
				m_Window = glfwCreateWindow(m_WindowedWidth, m_WindowedHeight, m_Title, NULL, NULL);
				m_Width = m_WindowedWidth;
				m_Height = m_WindowedHeight;
			}
			break;
		}
		default:
			break;
		}
		
		if (!m_Window) {
			glfwTerminate();
			E3DShowError("failed to create glfw window");
		}

		if (!m_Fullscreen) {
			m_WindowedWidth = m_Width;
			m_WindowedHeight = m_Height;
		}

		glfwMakeContextCurrent(m_Window);
		
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			E3DShowError("Failed to initialize glew");

		E3DPrint("OpenGL " << glGetString(GL_VERSION));
		E3DPrint(glGetString(GL_RENDERER) << " " << glGetString(GL_VENDOR));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSwapInterval((int)m_Vsync);

		glfwSetScrollCallback(m_Window, ScrollCallback);
		glfwSetWindowSizeCallback(m_Window, WindowSizeCallback);
		glfwSetWindowFocusCallback(m_Window, WindowFocusCallback);

		if(!m_FirstRun)
			RefreshContext();

		m_FirstRun = false;
		m_Focused = true;
	}

	void RenderWindow::RefreshContext()
	{
		if (m_Renderer->GetCamera())
			m_Renderer->GetCamera()->RefreshContext();

		for (auto batch : m_Renderer->m_Batches)
			batch->RefreshContext();
		Input::RefreshContext();
	}

	bool RenderWindow::IsOpen()
	{
		return !glfwWindowShouldClose(m_Window);
	}

	int RenderWindow::GetWidth() const
	{
		return m_Width;
	}

	int RenderWindow::GetHeight() const
	{
		return m_Height;
	}

	bool RenderWindow::GetVsync() const
	{
		return m_Vsync;
	}

	bool RenderWindow::IsFocused() const
	{
		return m_Focused;
	}

	void RenderWindow::SetVsync(bool enabled)
	{
		m_Vsync = enabled;
		glfwSwapInterval((int)m_Vsync);
	}

	void RenderWindow::Clear()
	{
		if (m_Renderer->GetCamera())
		{
			const glm::vec4& cc = m_Renderer->GetCamera()->GetClearColor();
			glClearColor(cc.r, cc.g, cc.b, cc.a);
		}
		else
		{
			glClearColor(0, 0, 0, 1);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderWindow::Swap()
	{
		Renderer::GetInstance()->Update();
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void RenderWindow::Destroy()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void RenderWindow::SetFullscreen(bool fullscreen)
	{
		if (!m_Fullscreen && fullscreen)
			CreateRenderWindow(ScreenMode::Fullscreen);
		else if (m_Fullscreen && !fullscreen)
			CreateRenderWindow(ScreenMode::Windowed);
		m_Fullscreen = fullscreen;
	}

	void RenderWindow::SetResolution(int width, int height)
	{
		glfwSetWindowSize(m_Window, width, height);
		RefreshContext();
	}

	bool RenderWindow::IsFullscreen() const
	{
		return m_Fullscreen;
	}
};