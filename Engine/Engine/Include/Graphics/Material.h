#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Common/Asset.h"

namespace E3D
{
	class Shader;
	class Texture;
	
	enum TextureType
	{
		Diffuse = GL_TEXTURE0,
		Specular = GL_TEXTURE1
	};

	class Material : public Asset
	{
		friend class MeshBatch;
		friend class Light;
		friend class DirectionalLight;
		friend class PointLight;
		friend class SpotLight;
		friend class AssetLoader;

	public:

		static Material* Default();
		static Material* GeoRenderNormals();

		void SetTexture(Texture* texture, const TextureType& textureType);
		Texture* GetTexture();
		Texture* GetTexture(const TextureType& textureType);

		void SetVertexShader(VertexShader* vertexShader);
		void SetFragmentShader(FragmentShader* fragmentShader);
		void SetGeometryShader(GeometryShader* geometryShader);

		VertexShader* GetVertexShader() const;
		FragmentShader* GetFragmentShader() const;
		GeometryShader* GetGeometryShader() const;

		void SetColor(const glm::vec4& color);
		const glm::vec4& GetColor() const;

		void SetShaderParam(const char* uniformName, int value);
		void SetShaderParam(const char* uniformName, float value);
		void SetShaderParam(const char* uniformName, double value);
		void SetShaderParam(const char* uniformName, float x, float y, float z);
		void SetShaderParam(const char* uniformName, float x, float y, float z, float w);
		void SetShaderParam(const char* uniformName, const glm::vec3& v3);
		void SetShaderParam(const char* uniformName, const glm::vec4& v4);
		void SetShaderParam(const char* uniformName, const glm::mat4& m4);
		// TODO: add more set shader param functions...

		void Bind();
		void UnBind();
		void InitParams();
		
	private:
		Material(const std::string& name);
		virtual ~Material();

		void RefreshShaders();
		void MapAttributes();
		void DisableVertexAttribArrays();

		std::map<TextureType, Texture*> m_Textures;
		glm::vec4 m_Color;

		GLint m_PositionAttribID;
		GLint m_ColorAttribID;
		GLint m_TextureAttribID;
		GLint m_NormalAttribID;

		VertexShader* m_VertexShader;
		FragmentShader* m_FragmentShader;
		GeometryShader* m_GeometryShader;

		GLuint m_ShaderProgram;
		GLint m_ModelMatrixHandle;
		GLint m_ViewMatrixHandle;
		GLint m_ProjectMatrixHandle;

	};
};