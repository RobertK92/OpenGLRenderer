#include "EnginePCH.h"
#include "Graphics/SpotLight.h"
#include "Graphics/Material.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"

namespace E3D
{

	SpotLight::SpotLight()
		: Light()
	{
		m_SpotLightIndex = _SpotLightCount;
		_SpotLightCount++;

		m_Cutoff = glm::cos(glm::radians(12.5f));
		m_OuterCutoff = glm::cos(glm::radians(17.5f));
	}

	void SpotLight::Initialise()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
			ForceUpdateMaterial(mat);
	}

	void SpotLight::ForceUpdateMaterial(Material* material)
	{
		material->Bind();
		material->SetShaderParam("spotLightCount", _SpotLightCount);
		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "intensity").c_str(), m_Intensity);
		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "ambient").c_str(), m_Ambient);
		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "diffuse").c_str(), m_Diffuse);
		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "specular").c_str(), m_Specular);

		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "direction").c_str(), m_Entity->GetForward());
		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "position").c_str(), m_Entity->GetPosition());
		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "cutoff").c_str(), m_Cutoff);
		material->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "outerCutoff").c_str(), m_OuterCutoff);
		material->UnBind();
	}

	SpotLight::~SpotLight()
	{
		_SpotLightCount--;
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam("spotLightCount", _SpotLightCount);
			mat->UnBind();
		}
	}

	float SpotLight::GetCutoff() const
	{
		return m_Cutoff;
	}

	float SpotLight::GetOuterCutoff() const
	{
		return m_OuterCutoff;
	}

	void SpotLight::SetCutoff(float cutoff)
	{
		if (m_Cutoff != cutoff) {
			m_Cutoff = glm::cos(glm::radians(cutoff));
			OnPropertyChanged();
		}
	}

	void SpotLight::SetOuterCutoff(float outerCutoff)
	{
		if (m_OuterCutoff != outerCutoff) {
			m_OuterCutoff = glm::cos(glm::radians(outerCutoff));
			OnPropertyChanged();
		}
	}

	void SpotLight::OnTransformed()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "direction").c_str(), m_Entity->GetForward());
			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "position").c_str(), m_Entity->GetPosition());
			mat->UnBind();
		}
	}

	void SpotLight::OnPropertyChanged()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "intensity").c_str(), m_Intensity);
			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "ambient").c_str(), m_Ambient);
			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "diffuse").c_str(), m_Diffuse);
			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "specular").c_str(), m_Specular);

			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "cutoff").c_str(), m_Cutoff);
			mat->SetShaderParam(ConstructUniformName("sLight", m_SpotLightIndex, "outerCutoff").c_str(), m_OuterCutoff);
			mat->UnBind();
		}
	}

	int SpotLight::_SpotLightCount = 0;

};