#pragma once

#include "Vertex.h"
#include "Common/Component.h"
#include "Material.h"

namespace E3D
{
	class MeshBatch;
	class RenderWindow;	
	class Shader;

	class Mesh : public Component
	{
		friend class MeshBatch;
		friend class Model;
		
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material* material, bool forceNewBatch);

		virtual ~Mesh();
		
		MeshBatch* GetBatch() const;
		const std::vector<Vertex>& GetVertices() const;
		const std::vector<GLuint>& GetIndices() const;

		int GetBufferOffset() const;

		Material* GetMaterial() const;
		void SetMaterial(Material* material);

		void SetShowNormals(bool show);
		bool GetShowNormals() const;

	private:
		MeshBatch* FindSuitableBatch();

		MeshBatch* m_Batch;
		Material* m_Material;

		bool m_ShowNormals;
		int m_BufferOffset;
		std::string m_ParentName;

		std::vector<Vertex> m_Vertices;
		std::vector<GLuint> m_Indices;
	};	
}