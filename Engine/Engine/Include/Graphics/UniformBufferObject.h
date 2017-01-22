#pragma once

namespace E3D
{
	struct Uniform
	{
		GLsizeiptr size;
		GLsizeiptr offset;
		void* data;
	};

	class UniformBufferObject
	{
		friend class Material;
	public:
		UniformBufferObject(GLchar* uniformBlockName, GLsizeiptr bufferSize, GLuint uniformBinding);
		virtual ~UniformBufferObject();

		void AddUniform(GLsizeiptr uniformSize, GLsizeiptr bufferOffset, void* uniformData);
		void AddUniform(Uniform* uniform);

		const std::vector<Uniform*>& GetUniforms() const;
		GLuint GetHandle() const;
		
	private:
		GLuint m_Ubo;
		std::vector<Uniform*> m_Uniforms;
		static std::map<GLuint, GLchar*> _BlockBindings;
	};
};