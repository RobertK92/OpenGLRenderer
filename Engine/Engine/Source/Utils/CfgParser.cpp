#include "EnginePCH.h"
#include "Utils/CfgParser.h"

namespace E3D
{
	CfgParser::CfgParser(const std::string& cfgFile)
	{
		std::fstream file;
		file.open(cfgFile);

		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
				if(line[0] == '#' || line.empty() || line[0] == '[')
					continue;
				
				size_t delim = line.find('=');
				std::string key = line.substr(0, delim);
				std::string val = line.erase(0, delim + 1);
				AddCorrectType(key, val);
			}
			file.close();
		}
		else
		{
			std::string err = "Failed to open Config file: ";
			err.append(cfgFile);
			E3DShowError(err.c_str());
		}
	}

	void CfgParser::AddCorrectType(std::string key, std::string val)
	{
		int count = 0;
		bool hasAlpha = false;
		for (char c : val) {
			if (!isdigit(c)) {
				if (((c == 'f' || c == 'F') && count == val.size() - 1) || 
					(c == '.' && isdigit(val[0]))) 
				{
					count++;
					continue;
				}
				hasAlpha = true;
				break;
			}
			count++;
		}
		
		// bool check
		if (hasAlpha)
		{
			std::string lowercVal = val;
			std::transform(lowercVal.begin(), lowercVal.end(), lowercVal.begin(), ::tolower);
			if (lowercVal == "false")
			{
				m_Bools.insert(std::pair<std::string, bool>(key, false));
				return;
			}
			else if (lowercVal == "true")
			{
				m_Bools.insert(std::pair<std::string, bool>(key, true));
				return;
			}
		}

		// string check
		if (hasAlpha || (val.size() == 1 && (val[0] == 'f' || val[0] == 'F')))
		{
			m_Strings.insert(std::pair<std::string, std::string>(key, val));
			return;
		}

		// int check
		if (val.size() == 1 || val.find('.') == std::string::npos)
		{
			m_Ints.insert(std::pair<std::string, int>(key, std::stoi(val)));
			return;
		}

		// then it must be a float
		m_Floats.insert(std::pair<std::string, float>(key, std::stof(val)));
		return;
	}

	int CfgParser::GetInt32(const std::string& field)
	{
		return GetField<int>(m_Ints, field);
	}

	std::string CfgParser::GetString(const std::string& field)
	{
		return GetField<std::string>(m_Strings, field);
	}

	float CfgParser::GetFloat(const std::string& field)
	{
		return GetField<float>(m_Floats, field);
	}

	bool CfgParser::GetBool(const std::string& field)
	{
		return GetField<bool>(m_Bools, field);
	}

	void CfgParser::NoSuchField(const std::string& field)
	{
		std::string err = "CfgParser failed to parse for field: '";
		err.append(field);
		err.append("' caused by misspelled field name or incorrect value type");
		E3DShowError(err.c_str());
	}

};