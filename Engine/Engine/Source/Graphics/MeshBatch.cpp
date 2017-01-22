#include "EnginePCH.h"
#include "Graphics/MeshBatch.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"
#include "Common/AssetLoader.h"
#include "Common/Config.h"

namespace E3D
{
	MeshBatch::MeshBatch(Material* material)
	{
		m_Material = material;
		m_IndexCount = 0;
		m_VertexCount = 0;
		m_Vao = 0;
		m_Vbo = 0;
		m_Ibo = 0;

		m_MaxVertices = 65536;
		m_MaxIndices = 262144;
		m_MaterialNormals = Material::GeoRenderNormals();

		Init();
	}

	MeshBatch::~MeshBatch()
	{
	}

	Material* MeshBatch::GetMaterial() const
	{
		return m_Material;
	}

	int MeshBatch::GetVertexCount() const
	{
		return (int)m_VertexCount;
	}

	int MeshBatch::GetIndexCount() const
	{
		return (int)m_IndexCount;
	}

	void MeshBatch::RefreshContext()
	{
		ClearData();
		Destroy();
		Init();
		for (auto mesh : m_Meshes)
		{
			BufferMesh(mesh);
		}
	}

	void MeshBatch::ClearData()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_MaxVertices * sizeof(Vertex), NULL);
		m_VertexCount = 0;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_MaxIndices * sizeof(GLuint), NULL);
		m_IndexCount = 0;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void MeshBatch::Init()
	{
		// create & bind Vertex array object
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		// create VBO & IBO
		glGenBuffers(1, &m_Vbo);
		glGenBuffers(1, &m_Ibo);

		// bind & populate VBO & IBO
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, m_MaxVertices * sizeof(Vertex), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MaxIndices * sizeof(GLuint), NULL, GL_STATIC_DRAW);
		
		m_Material->MapAttributes();

		// unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_Material->DisableVertexAttribArrays();

		m_Renderer = Renderer::GetInstance();
	}

	void MeshBatch::Inject(Mesh* mesh)
	{
		// no room in current batch, create and assign new batch
		if (mesh->m_Vertices.size() >= (m_MaxVertices - m_VertexCount) || mesh->m_Indices.size() >= (m_MaxIndices - m_IndexCount))
		{
			mesh->m_Batch = m_Renderer->CreateBatch(m_Material);
			
			if (mesh->m_Vertices.size() >= mesh->m_Batch->m_MaxVertices || mesh->m_Indices.size() >= mesh->m_Batch->m_MaxVertices)
			{
				mesh->m_Batch->Stretch(mesh->m_Vertices.size() + 1, mesh->m_Indices.size() + 1);
			}

			mesh->m_Batch->Inject(mesh);
		}
		else
		{
			BufferMesh(mesh);
			m_Meshes.push_back(mesh);	
		}
	}

	void MeshBatch::Eject(Mesh* mesh)
	{
		m_Meshes.erase(std::remove(m_Meshes.begin(), m_Meshes.end(), mesh), m_Meshes.end());
		ClearData();

		if (m_Meshes.size() == 0)
		{
			Destroy();
		}
		else 
		{
			for (auto m : m_Meshes)
			{
				BufferMesh(m);
			}
		}
	}

	void MeshBatch::BufferMesh(Mesh* mesh)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_VertexCount, mesh->m_Vertices.size() * sizeof(Vertex), &mesh->m_Vertices[0]);
		m_VertexCount += mesh->m_Vertices.size();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_IndexCount, mesh->m_Indices.size() * sizeof(GLuint), &mesh->m_Indices[0]);
		mesh->m_BufferOffset = m_IndexCount;
		m_IndexCount += mesh->m_Indices.size();

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void MeshBatch::Stretch(uint maxVertices, uint maxIndices)
	{
		m_MaxVertices = maxVertices;
		m_MaxIndices = maxIndices;
		
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, m_MaxVertices * sizeof(Vertex), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MaxIndices * sizeof(GLuint), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void MeshBatch::Display()
	{
		glBindVertexArray(m_Vao);
		
		m_Material->Bind();

		for (uint i = 0; i < m_Meshes.size(); ++i)
		{
			if (m_Renderer->GetCamera()) 
			{
				m_Material->SetShaderParam("view", m_Renderer->GetCamera()->GetEntity()->GetGlobalMatrix());
				m_Material->SetShaderParam("projection", m_Renderer->GetCamera()->GetProjectionMatrix());
			}
			m_Material->SetShaderParam("model", m_Meshes[i]->GetEntity()->GetGlobalMatrix());
			glDrawElements(GL_TRIANGLES, m_Meshes[i]->GetIndices().size(), GL_UNSIGNED_INT, E3D_BUFFER_OFFSET(m_Meshes[i]->m_BufferOffset));

			if (Config::ShowNormals())
			{
				m_Material->UnBind();
				m_MaterialNormals->Bind();

				if (m_Renderer->GetCamera())
				{
					m_MaterialNormals->SetShaderParam("view", m_Renderer->GetCamera()->GetEntity()->GetGlobalMatrix());
					m_MaterialNormals->SetShaderParam("projection", m_Renderer->GetCamera()->GetProjectionMatrix());
				}
				m_MaterialNormals->SetShaderParam("model", m_Meshes[i]->GetEntity()->GetGlobalMatrix());
				glDrawElements(GL_TRIANGLES, m_Meshes[i]->GetIndices().size(), GL_UNSIGNED_INT, E3D_BUFFER_OFFSET(m_Meshes[i]->m_BufferOffset));

				m_MaterialNormals->UnBind();
				m_Material->Bind();
			}
		}

		m_Material->UnBind();
		glBindVertexArray(0);
	}

	void MeshBatch::Destroy()
	{
		glDeleteVertexArrays(1, &m_Vao);
		glDeleteBuffers(1, &m_Vbo);
		glDeleteBuffers(1, &m_Ibo);
		m_Renderer->DestroyBatch(this);
	}
};