#include "EnginePCH.h"
#include "Graphics/UniformBufferObject.h"
#include "Graphics/Renderer.h"

namespace E3D
{
	UniformBufferObject::UniformBufferObject(GLchar* uniformBlockName, GLsizeiptr bufferSize, GLuint uniformBinding)
	{
		if (_BlockBindings.find(uniformBinding) != _BlockBindings.end())
		{
			std::string err = "Uniform block binding: '";
			err.append(std::to_string(uniformBinding));
			err.append("' already registered, choose a different binding index");
			E3DShowError(err.c_str());
		}

		glGenBuffers(1, &m_Ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_Ubo);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, uniformBinding, m_Ubo, 0, bufferSize);

		Renderer::GetInstance()->m_UniformBufferObjects.push_back(this);
		_BlockBindings.insert(std::pair<GLuint, GLchar*>(uniformBinding, uniformBlockName));
	}

	UniformBufferObject::~UniformBufferObject()
	{
		while (!m_Uniforms.empty()) 
		{
			delete m_Uniforms.back();
			m_Uniforms.pop_back();
		}
		_BlockBindings.erase(m_Ubo);
	}

	void UniformBufferObject::AddUniform(Uniform* uniform)
	{
		m_Uniforms.push_back(uniform);
	}

	void UniformBufferObject::AddUniform(GLsizeiptr uniformSize, GLsizeiptr bufferOffset, void* uniformData)
	{
		Uniform* uniform = new Uniform();
		uniform->data = uniformData;
		uniform->offset = bufferOffset;
		uniform->size = uniformSize;
		m_Uniforms.push_back(uniform);
	}

	const std::vector<Uniform*>& UniformBufferObject::GetUniforms() const
	{
		return m_Uniforms;
	}

	GLuint UniformBufferObject::GetHandle() const
	{
		return m_Ubo;
	}

	std::map<GLuint, GLchar*> UniformBufferObject::_BlockBindings;

};