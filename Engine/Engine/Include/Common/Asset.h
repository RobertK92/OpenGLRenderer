#pragma once

namespace E3D
{
	class Asset
	{
		friend class AssetLoader;
	public:
		Asset(const std::string& name);
		virtual ~Asset();

		const std::string& GetName() const;

	private:
		std::string m_Name;
	};
};