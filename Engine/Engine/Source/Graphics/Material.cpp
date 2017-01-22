#include "EnginePCH.h"
#include "Graphics/Material.h"
#include "Graphics/Vertex.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Graphics/UniformBufferObject.h"
#include "Graphics/Light.h"
#include "Common/AssetLoader.h"

namespace E3D
{
	Material* Material::Default()
	{
		static Material* defMat;
		if (defMat == nullptr) 
			defMat = AssetLoader::Load<Material>("MAT_Default");
		return defMat;
	}

	Material* Material::GeoRenderNormals()
	{
		static Material* geoMat;
		if (geoMat == nullptr) 
		{
			geoMat = AssetLoader::Load<Material>("MAT_GeoRenderNormals");
			geoMat->SetVertexShader(AssetLoader::Load<VertexShader>("Data/Shaders/RenderNormals.vert"));
			geoMat->SetFragmentShader(AssetLoader::Load<FragmentShader>("Data/Shaders/RenderNormals.frag"));
			geoMat->SetGeometryShader(AssetLoader::Load<GeometryShader>("Data/Shaders/RenderNormals.geom"));
		}
		return geoMat;
	}

	Material::Material(const std::string& name)
		: Asset(name)
	{
		m_VertexShader = VertexShader::Phong();
		m_FragmentShader = FragmentShader::Phong();
		m_GeometryShader = nullptr;

		Renderer::GetInstance()->GetMaterials().push_back(this);

		m_ShaderProgram = glCreateProgram();
		for (auto binding : UniformBufferObject::_BlockBindings)
		{
			GLuint blockIndex = glGetUniformBlockIndex(m_ShaderProgram, binding.second);
			glUniformBlockBinding(m_ShaderProgram, blockIndex, binding.first);
		}

		RefreshShaders();
		InitParams();

		for (auto light : Renderer::GetInstance()->GetLights())
			light->ForceUpdateMaterial(this);
	}

	Material::~Material()
	{
		Renderer::GetInstance()->GetMaterials().erase(
			std::remove(
				Renderer::GetInstance()->GetMaterials().begin(), 
				Renderer::GetInstance()->GetMaterials().end(), this));
		glDeleteProgram(m_ShaderProgram);
	}

	void Material::InitParams()
	{
		Bind();
		SetShaderParam("material.color", 1.f, 1.f, 1.f, 1.f);
		SetShaderParam("material.diffuse", 0);
		SetShaderParam("material.specular", 1);
		SetShaderParam("material.shininess", 32.f);
		UnBind();
	}

	void Material::SetTexture(Texture* texture, const TextureType& textureType)
	{
		if (m_Textures.find(textureType) != m_Textures.end())
			m_Textures[textureType] = texture;
		else
			m_Textures.insert(std::pair<TextureType, Texture*>(textureType, texture));

		Bind();
		glActiveTexture(textureType);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			texture->GetWidth(),
			texture->GetHeight(),
			0, GL_RGBA, GL_UNSIGNED_BYTE,
			texture->GetPixelBuffer());
		UnBind();
	}

	Texture* Material::GetTexture(const TextureType& textureType)
	{
		if (m_Textures.find(textureType) != m_Textures.end())
			return m_Textures[textureType];
		return nullptr;
	}

	Texture* Material::GetTexture()
	{
		if (m_Textures.find(TextureType::Diffuse) != m_Textures.end())
			return m_Textures[TextureType::Diffuse];
		return nullptr;
	}

	void Material::SetVertexShader(VertexShader* vertexShader)
	{
		m_VertexShader = vertexShader;
		RefreshShaders();
	}

	void Material::SetFragmentShader(FragmentShader* fragmentShader)
	{
		m_FragmentShader = fragmentShader;
		RefreshShaders();
	}

	void Material::SetGeometryShader(GeometryShader* geometryShader)
	{
		m_GeometryShader = geometryShader;
		RefreshShaders();
	}

	VertexShader* Material::GetVertexShader() const
	{
		return m_VertexShader;
	}

	FragmentShader* Material::GetFragmentShader() const
	{
		return m_FragmentShader;
	}

	GeometryShader* Material::GetGeometryShader() const
	{
		return m_GeometryShader;
	}

	void Material::SetColor(const glm::vec4& color)
	{
		m_Color = color;
		
	}

	const glm::vec4& Material::GetColor() const
	{
		return m_Color;
	}

	void Material::SetShaderParam(const char* uniformName, const glm::vec3& v3)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniform3f(loc, v3.x, v3.y, v3.z);
	}

	void Material::SetShaderParam(const char* uniformName, const glm::vec4& v4)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniform4f(loc, v4.r, v4.g, v4.b, v4.a);
	}

	void Material::SetShaderParam(const char* uniformName, const glm::mat4& m4)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m4));
	}

	void Material::SetShaderParam(const char* uniformName, float x, float y, float z)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniform3f(loc, x, y, z);
	}

	void Material::SetShaderParam(const char* uniformName, float x, float y, float z, float w)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniform4f(loc, x, y, z, w);
	}

	void Material::SetShaderParam(const char* uniformName, float value)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniform1f(loc, value);
	}

	void Material::SetShaderParam(const char* uniformName, int value)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniform1i(loc, value);
	}

	void Material::SetShaderParam(const char* uniformName, double value)
	{
		GLint loc = glGetUniformLocation(m_ShaderProgram, uniformName);
		glUniform1d(loc, value);
	}

	void Material::Bind()
	{
		glUseProgram(m_ShaderProgram);
	
		for (auto tex : m_Textures)
		{
			glActiveTexture(tex.first);
			glBindTexture(GL_TEXTURE_2D, tex.second->GetHandle());
		}
		
/*
		Camera* cam = Renderer::GetInstance()->GetCamera();
		if (cam)
			SetShaderParam("viewerPos", cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z);*/
	}

	void Material::UnBind()
	{
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void Material::RefreshShaders()
	{	
		glAttachShader(m_ShaderProgram, m_VertexShader->GetHandle());
		glAttachShader(m_ShaderProgram, m_FragmentShader->GetHandle());

		if(m_GeometryShader)
			glAttachShader(m_ShaderProgram, m_GeometryShader->GetHandle());
		
		glLinkProgram(m_ShaderProgram);
		GLint linkStatus;
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &linkStatus);

		if (linkStatus != GL_TRUE)
		{
			GLint logLen;
			glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &logLen);
			GLchar* infoLog = new GLchar[logLen];
			glGetProgramInfoLog(m_ShaderProgram, logLen, NULL, infoLog);
			std::string infoStr = "Failed to link shader program \n";
			infoStr.append(infoLog);
			E3DShowError(infoStr.c_str());
			delete[] infoLog;
		}
		
		glDetachShader(m_ShaderProgram, m_VertexShader->GetHandle());
		glDetachShader(m_ShaderProgram, m_FragmentShader->GetHandle());

		if (m_GeometryShader)
			glDetachShader(m_ShaderProgram, m_GeometryShader->GetHandle());
	}
	
	void Material::MapAttributes()
	{
		m_PositionAttribID = glGetAttribLocation(m_ShaderProgram, "in_position");
		m_ColorAttribID = glGetAttribLocation(m_ShaderProgram, "in_color");
		m_TextureAttribID = glGetAttribLocation(m_ShaderProgram, "in_texCords");
		m_NormalAttribID = glGetAttribLocation(m_ShaderProgram, "in_normal");
		glVertexAttribPointer(m_PositionAttribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), E3D_MEMBER_OFFSET(Vertex, m_Pos));
		glEnableVertexAttribArray(m_PositionAttribID);
		glVertexAttribPointer(m_ColorAttribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), E3D_MEMBER_OFFSET(Vertex, m_Color));
		glEnableVertexAttribArray(m_ColorAttribID);
		glVertexAttribPointer(m_TextureAttribID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), E3D_MEMBER_OFFSET(Vertex, m_TexCord));
		glEnableVertexAttribArray(m_TextureAttribID);
		glVertexAttribPointer(m_NormalAttribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), E3D_MEMBER_OFFSET(Vertex, m_Normal));
		glEnableVertexAttribArray(m_NormalAttribID);
	}

	void Material::DisableVertexAttribArrays()
	{
		glDisableVertexAttribArray(m_PositionAttribID);
		glDisableVertexAttribArray(m_ColorAttribID);
		glDisableVertexAttribArray(m_TextureAttribID);
		glDisableVertexAttribArray(m_NormalAttribID);
	}

};
