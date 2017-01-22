#pragma once

#include "Light.h"

namespace E3D
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		virtual ~DirectionalLight();
		virtual void Initialise() override;

		virtual void OnTransformed() override;
		virtual void OnPropertyChanged() override;
		virtual void ForceUpdateMaterial(Material* material) override;

	private:
		int m_DirectionalLightIndex;
		static int _DirectionalLightCounter;
	};
};
