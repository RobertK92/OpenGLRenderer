#include "EnginePCH.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/UniformBufferObject.h"

namespace E3D 
{
	Camera::Camera() 
	{
		m_Fov = 60.0f;
		m_Near = 0.3f;
		m_Far = 1000.0f;
		m_Window = Renderer::GetInstance()->GetWindow();
		m_ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Camera::~Camera()
	{
		Renderer::GetInstance()->m_Camera = nullptr;
	}

	void Camera::Initialise()
	{
		m_Entity->SetPosition(glm::vec3(0, 0, 10));
		m_Entity->SetRotation(glm::quat());
		RefreshContext();

		Renderer::GetInstance()->m_Camera = this;
	}

	void Camera::RefreshContext()
	{
		SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
		SetProjectionRH(m_Fov, m_Window->GetWidth() / (float)m_Window->GetHeight(), m_Near, m_Far);
	}

	void Camera::SetViewport(int x, int y, int width, int height)
	{
		m_Viewport = glm::vec4(x, y, width, height);
		glViewport(x, y, width, height);
	}

	const glm::vec4& Camera::GetViewport() const
	{
		return m_Viewport;
	}

	void Camera::SetClipping(float camNear, float camFar)
	{
		SetProjectionRH(m_Fov, m_Window->GetWidth() / (float)m_Window->GetHeight(), camNear, camFar);
	}

	void Camera::SetFieldOfView(float fov)
	{
		m_Fov = glm::clamp(fov, 0.0f, 179.99f);
		SetProjectionRH(m_Fov, m_Window->GetWidth() / (float)m_Window->GetHeight(), m_Near, m_Far);
	}

	float Camera::GetFieldOfView() const
	{
		return m_Fov;
	}

	void Camera::SetProjectionRH(float fov, float aspectRatio, float zNear, float zFar)
	{
		m_Near = zNear;
		m_Far = zFar;
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	const glm::vec3& Camera::ScreenToWorld(const glm::vec2& screenPos)
	{
		E3DShowIssue("Camera.ScreenToWorld not implemented yet.");
	}

	const glm::vec2& Camera::WorldToScreen(const glm::vec3& worldPos)
	{
		E3DShowIssue("Camera.WorldToScreen not implemented yet.");
	}

	void Camera::SetClearColor(const glm::vec4& color)
	{
		m_ClearColor = color; 
	}

	const glm::vec4& Camera::GetClearColor() const
	{
		return m_ClearColor;
	}
	
	void Camera::OnTransformed()
	{
		m_Entity->m_LocalMatrix		= glm::inverse(m_Entity->m_LocalMatrix);
		m_Entity->m_GlobalMatrix	= glm::inverse(m_Entity->m_GlobalMatrix);
		m_Entity->m_Forward			= -m_Entity->m_Forward;
		m_Entity->m_Up				= -m_Entity->m_Up;
		m_Entity->m_Right			= -m_Entity->m_Right;
	}

	RenderWindow* Camera::m_Window;
};