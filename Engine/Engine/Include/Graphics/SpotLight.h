#pragma once
#include "Light.h"
namespace E3D
{
	class SpotLight : public Light
	{
	public:
		SpotLight();
		virtual ~SpotLight();
		virtual void Initialise()override;
		float GetCutoff() const;
		float GetOuterCutoff() const;

		void SetCutoff(float cutoff);
		void SetOuterCutoff(float outerCutoff);

		void virtual OnTransformed() override;
		void virtual OnPropertyChanged() override;
		void virtual ForceUpdateMaterial(Material* material) override;

	private:
		int m_SpotLightIndex;

		float m_Cutoff;
		float m_OuterCutoff;

		static int _SpotLightCount;
	};
};