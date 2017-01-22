#include "EnginePCH.h"
#include "Graphics/Shader.h"
#include "Common/AssetLoader.h"

namespace E3D
{
	std::string* shaderCache = nullptr;
	//////////////////////////////////////////////////////////////////////////
	// Shader Base
	//////////////////////////////////////////////////////////////////////////

	Shader::Shader(const std::string& shaderFile)
		: Asset(shaderFile)
	{
	}

	Shader::~Shader()
	{
		if (shaderCache) {
			delete shaderCache;
			shaderCache = nullptr;
		}
	}

	void Shader::Compile(const char* shaderString)
	{
		GLuint shaderHandle = glCreateShader(m_ShaderType);
		const GLchar* sources[] = { shaderString };
		glShaderSource(shaderHandle, 1, sources, NULL);
		glCompileShader(shaderHandle);

		GLint compileStatus;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus != GL_TRUE)
		{
			GLint logLen;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLen);
			GLchar* infoLog = new GLchar[logLen];
			glGetShaderInfoLog(shaderHandle, logLen, NULL, infoLog);
			std::string infoStr = "Failed to compile shader \n";
			infoStr.append(infoLog);
			E3DShowError(infoStr.c_str());
			glDeleteShader(shaderHandle);
			delete[] infoLog;
		}

		m_ShaderHandle = shaderHandle;

		delete shaderCache;
		shaderCache = nullptr;
	}

	const char* Shader::Load(const char* shaderFilePath)
	{
		std::fstream file;
		file.open(shaderFilePath, std::ios::in);

		if (!file.is_open()) 
		{
			std::string err = "Failed to load shader file from path: ";
			err.append(shaderFilePath);
			E3DShowError(err.c_str());
			return nullptr;
		}
		
		shaderCache = new std::string(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
		file.close();
		return shaderCache->c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	// Vertex Shader
	//////////////////////////////////////////////////////////////////////////
	VertexShader::VertexShader(const std::string& shaderFile)
		: Shader(shaderFile) 
	{
		m_ShaderType = GL_VERTEX_SHADER;
		Compile(Load(shaderFile.c_str()));
	}

	VertexShader::~VertexShader() 
	{
	}

	VertexShader* VertexShader::Unlit()
	{
		return AssetLoader::Load<VertexShader>(E3D_VERTEX_SHADER_UNLIT_PATH);
	}

	VertexShader* VertexShader::Phong()
	{
		return AssetLoader::Load<VertexShader>(E3D_VERTEX_SHADER_PHONG_PATH);
	}

	//////////////////////////////////////////////////////////////////////////
	// Fragment Shader
	//////////////////////////////////////////////////////////////////////////
	FragmentShader::FragmentShader(const std::string& shaderFile)
		: Shader(shaderFile) 
	{
		m_ShaderType = GL_FRAGMENT_SHADER;
		Compile(Load(shaderFile.c_str()));
	}

	FragmentShader::~FragmentShader() 
	{
	}

	FragmentShader* FragmentShader::Unlit()
	{
		return AssetLoader::Load<FragmentShader>(E3D_FRAGMENT_SHADER_UNLIT_PATH);
	}

	FragmentShader* FragmentShader::Phong()
	{
		return AssetLoader::Load<FragmentShader>(E3D_FRAGMENT_SHADER_PHONG_PATH);
	}

	//////////////////////////////////////////////////////////////////////////
	// Geometry Shader
	//////////////////////////////////////////////////////////////////////////
	GeometryShader::GeometryShader(const std::string& shaderFile)
		: Shader(shaderFile)
	{
		m_ShaderType = GL_GEOMETRY_SHADER;
		Compile(Load(shaderFile.c_str()));
	}

	GeometryShader::~GeometryShader()
	{

	}

	GeometryShader* GeometryShader::RenderNormals()
	{
		return AssetLoader::Load<GeometryShader>(E3D_GEOMETRY_SHADER_RENDER_NORMALS_PATH);
	}

};