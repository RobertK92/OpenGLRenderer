#pragma once
#include "Light.h"
namespace E3D
{
	class PointLight : public Light
	{
	public:
		PointLight();
		virtual ~PointLight();
		virtual void Initialise() override;
		float GetConstant() const;
		float GetLinear() const;
		float GetQuadratic() const;

		void SetConstant(float constant);
		void SetLinear(float linear);
		void SetQuadratic(float quadratic);

		virtual void OnTransformed() override;
		virtual void OnPropertyChanged() override;
		virtual void ForceUpdateMaterial(Material* material) override;

	private:

		int m_PointLightIndex;
		float m_Constant;
		float m_Linear;
		float m_Quadratic;

		static int _PointLightCounter;
	};
};