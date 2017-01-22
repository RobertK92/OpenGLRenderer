#pragma once


namespace E3D
{
	enum ScreenMode
	{
		Fullscreen,
		FullscreenWindowed,
		Windowed
	};

	class Renderer;
	class RenderWindow
	{
		friend class Mesh;
		friend class Camera;
		friend class Game;
	public:
		
		RenderWindow(int width, int height, char* title, bool vsync = true);

		bool IsOpen();
		void Clear();
		void Swap();
		void Destroy();

		void SetFullscreen(bool fullscreen);
		void SetResolution(int width, int height);
		void SetVsync(bool enabled);

		bool IsFullscreen() const;
		int GetWidth() const;
		int GetHeight() const;
		bool GetVsync() const;
		bool IsFocused() const;
		GLFWwindow* m_Window;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Callbacks
		//////////////////////////////////////////////////////////////////////////
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
		static void WindowFocusCallback(GLFWwindow* window, int focused);
		//////////////////////////////////////////////////////////////////////////

		void RefreshContext();
		void CreateRenderWindow(const ScreenMode& screenMode);

		char* m_Title;
		int m_Width;
		int m_Height;

		int m_WindowedWidth;
		int m_WindowedHeight;
		ScreenMode m_ScreenMode;
		
		bool m_FirstRun;
		bool m_Focused;
		bool m_Fullscreen;
		bool m_Vsync;
		Renderer* m_Renderer;
	};
};