#include "EnginePCH.h"
#include "Graphics/Light.h"
#include "Common/Transformable.h"
#include "Graphics/Material.h"
#include "Graphics/Renderer.h"

namespace E3D
{
	Light::Light()
	{
		m_Intensity = 1.0f;
		m_Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		m_Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		m_Specular = glm::vec3(1.0f, 1.0f, 1.0f);
		Renderer::GetInstance()->GetLights().push_back(this);
	}

	Light::~Light()
	{
		Renderer::GetInstance()->GetLights().erase(
			std::remove(
				Renderer::GetInstance()->GetLights().begin(),
				Renderer::GetInstance()->GetLights().end(), this));
	}

	std::string Light::ConstructUniformName(const std::string& uniform, int index, const std::string& field)
	{
		std::string iStr = std::to_string(index);
		std::string uni = uniform;
		return (uni.append("[").append(iStr).append("]").append(".").append(field));
	}

	float Light::GetIntensity() const
	{
		return m_Intensity;
	}

	const glm::vec4& Light::GetColor() const
	{
		return m_Color;
	}

	const glm::vec3 Light::GetAmbient() const
	{
		return m_Ambient;
	}

	const glm::vec3& Light::GetDiffuse() const
	{
		return m_Diffuse;
	}

	const glm::vec3& Light::GetSpecular() const
	{
		return m_Specular;
	}

	void Light::SetIntensity(float intensity)
	{
		if (intensity != m_Intensity) {
			m_Intensity = glm::clamp(intensity, 0.0f, 1.0f);
			OnPropertyChanged();
		}
	}

	void Light::SetColor(const glm::vec4& color)
	{
		if (m_Color != color) {
			m_Color = color;
			OnPropertyChanged();
		}
	}

	void Light::SetAmbient(const glm::vec3& ambient)
	{
		if (m_Ambient != ambient) {
			m_Ambient = ambient;
			OnPropertyChanged();
		}
	}

	void Light::SetDiffuse(const glm::vec3& diffuse)
	{
		if (m_Diffuse != diffuse) {
			m_Diffuse = diffuse;
			OnPropertyChanged();
		}
	}

	void Light::SetSpecular(const glm::vec3& specular)
	{
		if (m_Specular != specular) {
			m_Specular = specular;
			OnPropertyChanged();
		}
	}

};