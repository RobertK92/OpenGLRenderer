#include "EnginePCH.h"
#include "Common/Config.h"
#include "Utils/CfgParser.h"

namespace E3D
{

	void Config::Init()
	{
		CfgParser cfg = CfgParser("Data/Config.cfg");
		m_AASamples = cfg.GetInt32("AASamples");
		m_UseDynamicBatching = cfg.GetBool("DynamicBatching");
		m_DefaultScreenWidth = cfg.GetInt32("ScreenWidth");
		m_DefaultScreenHeight = cfg.GetInt32("ScreenHeight");
		m_ShowNormals = false;
	}

	int Config::AASamples()
	{
		return m_AASamples;
	}

	bool Config::UseDynamicBatching()
	{
		return m_UseDynamicBatching;
	}

	int Config::DefaultScreenWidth()
	{
		return m_DefaultScreenWidth;
	}

	int Config::DefaultScreenHeight()
	{
		return m_DefaultScreenHeight;
	}

	bool Config::ShowNormals()
	{
		return m_ShowNormals;
	}

	void Config::SetShowNormals(bool showNormals)
	{
		m_ShowNormals = showNormals;
	}

	bool Config::m_UseDynamicBatching;

	int Config::m_DefaultScreenWidth;

	int Config::m_DefaultScreenHeight;

	bool Config::m_ShowNormals;

	int Config::m_AASamples;

};