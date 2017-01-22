#pragma once

namespace E3D
{
	class Asset;
	class AssetLoader
	{
	public:
		template<class T> static T* Load(const std::string& asset)
		{
			if (m_Assets.find(asset) != m_Assets.end()) 
				return dynamic_cast<T*>(m_Assets[asset]);
			T* newT = new T(asset);
			if (newT != nullptr)
				m_Assets.insert(std::pair<std::string , Asset*>(asset, newT));
			return newT;
		}

	private:
		static std::map<std::string, Asset*> m_Assets;
	};
};