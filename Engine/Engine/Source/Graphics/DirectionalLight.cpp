#include "EnginePCH.h"
#include "Graphics/DirectionalLight.h"
#include "Graphics/Material.h"
#include "Graphics/Renderer.h"

namespace E3D
{
	DirectionalLight::DirectionalLight()
		: Light()
	{
		m_DirectionalLightIndex = _DirectionalLightCounter;
		_DirectionalLightCounter++;
	}

	void DirectionalLight::Initialise()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
			ForceUpdateMaterial(mat);
	}

	void DirectionalLight::ForceUpdateMaterial(Material* material)
	{
		material->Bind();
		material->SetShaderParam("dirLightCount", _DirectionalLightCounter);
		material->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "intensity").c_str(), m_Intensity);
		material->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "direction").c_str(), m_Entity->GetForward());
		material->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "ambient").c_str(), m_Ambient);
		material->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "diffuse").c_str(), m_Diffuse);
		material->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "specular").c_str(), m_Specular);
		material->UnBind();
	}

	DirectionalLight::~DirectionalLight()
	{
		_DirectionalLightCounter--;
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam("dirLightCount", _DirectionalLightCounter);
			mat->UnBind();
		}
	}

	void DirectionalLight::OnTransformed()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "direction").c_str(), m_Entity->GetForward());
			mat->UnBind();
		}
	}

	void DirectionalLight::OnPropertyChanged()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "intensity").c_str(), m_Intensity);
			mat->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "ambient").c_str(), m_Ambient);
			mat->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "diffuse").c_str(), m_Diffuse);
			mat->SetShaderParam(ConstructUniformName("dLight", m_DirectionalLightIndex, "specular").c_str(), m_Specular);
			mat->UnBind();
		}
	}

	int DirectionalLight::_DirectionalLightCounter = 0;

};