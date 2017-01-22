#include "EnginePCH.h"
#include "Graphics/Renderer.h"
#include "Graphics/MeshBatch.h"
#include "Graphics/UniformBufferObject.h"
#include "Common/Transformable.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"

namespace E3D {
	
	Renderer::Renderer()
	{

	}

	MeshBatch* Renderer::FindBatch(Mesh* mesh, Material* material)
	{
		for (auto batch : m_Batches)
		{
			if(batch->GetMaterial() == material)
			{
				if(batch->GetVertexCount() < (int)(batch->m_MaxVertices - (mesh->GetVertices().size() + 1)))
					return batch;
			}
		}
		return nullptr;
	}

	void Renderer::Update()
	{
		for (uint i = 0; i < m_TransformationStack.size(); i++)
			m_TransformationStack[i]->UpdateTransformationMatrix();
		
		UpdateUniformBuffers();
		for (auto batch : m_Batches)
		{
			batch->Display();
		}
	}

	MeshBatch* Renderer::CreateBatch(Material* material)
	{
		MeshBatch* batch = new MeshBatch(material);
		m_Batches.push_back(batch);
		return batch;
	}

	void Renderer::DestroyBatch(MeshBatch* batch)
	{
		m_Batches.erase(std::remove(m_Batches.begin(), m_Batches.end(), batch), m_Batches.end());
		delete batch;
	}

	RenderWindow* Renderer::GetWindow() const
	{
		return m_Window;
	}

	std::vector<Transformable*>& Renderer::GetTransformationStack() const
	{
		return m_TransformationStack;
	}

	std::vector<Material*>& Renderer::GetMaterials() const
	{
		return m_Materials;
	}

	std::vector<Light*>& Renderer::GetLights() const
	{
		return m_Lights;
	}

	Camera* Renderer::GetCamera() const
	{
		return m_Camera;
	}

	void Renderer::SetWindow(RenderWindow* window)
	{
		m_Window = window;
	}

	void Renderer::UpdateUniformBuffers()
	{
		for (auto ubo : m_UniformBufferObjects)
		{
			for (auto uniform : ubo->GetUniforms()) 
			{
				glBindBuffer(GL_UNIFORM_BUFFER, ubo->GetHandle());
				glBufferSubData(GL_UNIFORM_BUFFER, uniform->offset, uniform->size, uniform->data);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
		}

		/*// example: setting projection matrix to id mat
		glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Camera::GetMainCamera()->GetProjectionMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		// example: setting view matrix to id mat
		glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Camera::GetMainCamera()->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
	}

};
