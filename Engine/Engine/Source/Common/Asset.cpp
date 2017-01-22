#include "EnginePCH.h"
#include "Common/Asset.h"

namespace E3D
{
	Asset::Asset(const std::string& name)
	{
		m_Name = name;
	}

	Asset::~Asset() { }


	const std::string& Asset::GetName() const
	{
		return m_Name;
	}

};