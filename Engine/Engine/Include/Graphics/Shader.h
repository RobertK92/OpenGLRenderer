#pragma once

#include "Common/Asset.h"

namespace E3D
{
	class Shader : public Asset
	{
		friend AssetLoader;
	public:
		GLuint GetHandle() const { return m_ShaderHandle; }

	protected:
		Shader(const std::string& shaderFile);
		virtual ~Shader();

		void Compile(const char* shaderString);
		const char* Load(const char* shaderFilePath);

		GLuint m_ShaderHandle;
		GLenum m_ShaderType;
	};

	class VertexShader : public Shader
	{
		friend AssetLoader;
	public:
		static VertexShader* Unlit();
		static VertexShader* Phong();
	protected:
		VertexShader(const std::string& shaderFile);
		virtual ~VertexShader();
	};

	class FragmentShader : public Shader
	{
		friend AssetLoader;
	public:
		static FragmentShader* Unlit();
		static FragmentShader* Phong();
	protected:
		FragmentShader(const std::string& shaderFile);
		virtual ~FragmentShader();
	};

	class GeometryShader : public Shader
	{
		friend AssetLoader;
	public:
		static GeometryShader* RenderNormals();
	protected:
		GeometryShader(const std::string& shaderFile);
		virtual ~GeometryShader();
	};
}
