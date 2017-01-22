#pragma once

namespace E3D
{
	struct Vertex
	{
		glm::vec3 m_Pos;		// XYZ
		glm::vec3 m_Color;		// RGB
		glm::vec2 m_TexCord;	// UV
		glm::vec3 m_Normal;		// XYZ
	};
};