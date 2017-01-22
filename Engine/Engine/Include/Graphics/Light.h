#pragma once

#include "Common/Component.h"

namespace E3D
{
	class Light : public Component
	{
		friend class Material;
	public:
		float GetIntensity() const;
		const glm::vec4& GetColor() const;
		const glm::vec3 GetAmbient() const;
		const glm::vec3& GetDiffuse() const;
		const glm::vec3& GetSpecular() const;

		void SetIntensity(float intensity);
		void SetColor(const glm::vec4& color);
		void SetAmbient(const glm::vec3& ambient);
		void SetDiffuse(const glm::vec3& diffuse);
		void SetSpecular(const glm::vec3& specular);

	protected:
		Light();
		virtual ~Light();
		virtual void OnPropertyChanged() {}
		virtual void ForceUpdateMaterial(Material* material) {}

		std::string ConstructUniformName(const std::string& uniform, int index, const std::string& field);
		float m_Intensity;
		glm::vec4 m_Color;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};
};