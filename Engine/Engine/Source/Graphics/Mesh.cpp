#include "EnginePCH.h"
#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/MeshBatch.h"
#include "Graphics/Shader.h"
#include "Graphics/Material.h"
#include "Common/AssetLoader.h"

namespace E3D
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material* material, bool forceNewBatch)
	{
		m_Material = material;
		m_ShowNormals = false;

		m_BufferOffset = 0;
		m_Vertices = vertices;
		m_Indices = indices;

		m_Material = !m_Material ? Material::Default() : m_Material;
		
		if (forceNewBatch)
		{
			m_Batch = Renderer::GetInstance()->CreateBatch(m_Material);
		}
		else {
			m_Batch = FindSuitableBatch();
		}
		m_Batch->Inject(this);
	}

	Mesh::~Mesh()
	{
		m_Batch->Eject(this);
		m_Vertices.clear();
		m_Indices.clear();
	}

	MeshBatch* Mesh::GetBatch() const
	{
		return m_Batch;
	}

	const std::vector<Vertex>& Mesh::GetVertices() const
	{
		return m_Vertices;
	}

	const std::vector<GLuint>& Mesh::GetIndices() const
	{
		return m_Indices;
	}

	int Mesh::GetBufferOffset() const
	{
		return m_BufferOffset;
	}

	Material* Mesh::GetMaterial() const
	{
		return m_Material;
	}

	void Mesh::SetMaterial(Material* material)
	{
		m_Material = material;
		if(m_Batch != nullptr)
			m_Batch->m_Material = material;
	}

	bool Mesh::GetShowNormals() const
	{
		return m_ShowNormals;
	}

	void Mesh::SetShowNormals(bool show)
	{
		m_ShowNormals = show;
	}

	MeshBatch* Mesh::FindSuitableBatch()
	{
		MeshBatch* batch = Renderer::GetInstance()->FindBatch(this, m_Material);
		if (batch)
		{
			return batch;
		}
		else
		{
			return Renderer::GetInstance()->CreateBatch(m_Material);
		}
	}

	

};
