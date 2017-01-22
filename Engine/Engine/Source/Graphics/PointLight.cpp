#include "EnginePCH.h"
#include "Graphics/PointLight.h"
#include "Graphics/Material.h"
#include "Graphics/Renderer.h"

namespace E3D
{

	PointLight::PointLight()
		: Light()
	{
		m_Constant = 1.0f;
		m_Linear = 0.09f;
		m_Quadratic = 0.032f;

		m_PointLightIndex = _PointLightCounter;
		_PointLightCounter++;
	}

	void PointLight::Initialise()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
			ForceUpdateMaterial(mat);
	}

	void PointLight::ForceUpdateMaterial(Material* material)
	{
		material->Bind();
		material->SetShaderParam("pointLightCount", _PointLightCounter);
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "intensity").c_str(), m_Intensity);
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "position").c_str(), m_Entity->GetPosition());
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "ambient").c_str(), m_Ambient);
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "diffuse").c_str(), m_Diffuse);
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "specular").c_str(), m_Specular);
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "constant").c_str(), m_Constant);
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "linear").c_str(), m_Linear);
		material->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "quadratic").c_str(), m_Quadratic);
		material->UnBind();
	}

	PointLight::~PointLight()
	{
		_PointLightCounter--;
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam("pointLightCount", _PointLightCounter);
			mat->UnBind();
		}
	}

	void PointLight::OnTransformed()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "direction").c_str(), m_Entity->GetForward());
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "position").c_str(), m_Entity->GetPosition());
			mat->UnBind();
		}
	}

	void PointLight::OnPropertyChanged()
	{
		for (auto mat : Renderer::GetInstance()->GetMaterials())
		{
			mat->Bind();
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "intensity").c_str(), m_Intensity);
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "ambient").c_str(), m_Ambient);
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "diffuse").c_str(), m_Diffuse);
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "specular").c_str(), m_Specular);
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "constant").c_str(), m_Constant);
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "linear").c_str(), m_Linear);
			mat->SetShaderParam(ConstructUniformName("pLight", m_PointLightIndex, "quadratic").c_str(), m_Quadratic);
			mat->UnBind();
		}
	}


	float PointLight::GetConstant() const
	{
		return m_Constant;
	}

	float PointLight::GetLinear() const
	{
		return m_Linear;
	}

	float PointLight::GetQuadratic() const
	{
		return m_Quadratic;
	}

	void PointLight::SetConstant(float constant)
	{
		if (m_Constant != constant) {
			m_Constant = constant;
			OnPropertyChanged();
		}
	}

	void PointLight::SetLinear(float linear)
	{
		if (m_Linear != linear) {
			m_Linear = linear;
			OnPropertyChanged();
		}
	}

	void PointLight::SetQuadratic(float quadratic)
	{
		if (m_Quadratic != quadratic) {
			m_Quadratic = quadratic;
			OnPropertyChanged();
		}
	}

	int PointLight::_PointLightCounter = 0;

};