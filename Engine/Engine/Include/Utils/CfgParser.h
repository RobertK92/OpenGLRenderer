#pragma once

namespace E3D
{
	class CfgParser
	{
	public:
		CfgParser(const std::string& cfgFile);

		int GetInt32(const std::string& field);
		std::string GetString(const std::string& field);
		float GetFloat(const std::string& field);
		bool GetBool(const std::string& field);

	private:
		void AddCorrectType(std::string key, std::string val);
		void NoSuchField(const std::string& field);
		template<class T> T GetField(std::map<std::string, T>& _map, const std::string& field)
		{
			if (_map.find(field) != _map.end())
				return _map[field];
			NoSuchField(field);
			return NULL;
		}

		std::map<std::string, bool> m_Bools;
		std::map<std::string, int> m_Ints;
		std::map<std::string, float> m_Floats;
		std::map<std::string, std::string> m_Strings;
	};
};