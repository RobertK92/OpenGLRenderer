#pragma once

#include "Common/Component.h"

namespace E3D 
{
	class RenderWindow;

	class Camera : public Component
	{
		friend class RenderWindow;
	public:
		Camera();
		virtual ~Camera();
		virtual void Initialise() override;

		void  SetViewport(int x, int y, int width, int height);
		const glm::vec4&  GetViewport() const;

		void SetClipping(float camNear, float camFar);
		void SetFieldOfView(float fov);
		float GetFieldOfView() const;
		void  SetProjectionRH(float fov, float aspectRatio, float zNear, float zFar);

		const glm::mat4& GetProjectionMatrix() const;
		const glm::vec3& ScreenToWorld(const glm::vec2& screenPos);
		const glm::vec2& WorldToScreen(const glm::vec3& worldPos);

		void SetClearColor(const glm::vec4& color);
		const glm::vec4& GetClearColor() const;

		virtual void OnTransformed() override;

	protected:
		void RefreshContext();

		float m_Fov;
		float m_Near;
		float m_Far;

		glm::vec4 m_Viewport;
		glm::mat4 m_ProjectionMatrix;
		glm::vec4 m_ClearColor;
	
		static RenderWindow* m_Window;

	};
};