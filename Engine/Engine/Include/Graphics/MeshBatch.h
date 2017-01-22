#pragma once

#include "Vertex.h"

/*
#define R3D_MAX_VERTICES 65536
#define R3D_MAX_VBO_SIZE R3D_MAX_VERTICES * sizeof(Vertex) 


#define R3D_MAX_INDICES 65536
#define R3D_MAX_IBO_SIZE R3D_MAX_INDICES * sizeof(GLuint) 
*/

namespace E3D 
{
	class Camera;
	class Shader;
	class Mesh;
	class Material;

	class MeshBatch
	{
		friend class Renderer;
		friend class Mesh;
	public:
		MeshBatch(Material* material);
		~MeshBatch();

		void Destroy();
		Material* GetMaterial() const;
		int GetVertexCount() const;
		int GetIndexCount() const;
		
		void RefreshContext();

	private:
		void Init();
		void Display();
		void BufferMesh(Mesh* mesh);
		void Inject(Mesh* mesh);
		void Eject(Mesh* mesh);
		void ClearData();
		void Stretch(uint maxVertices, uint maxIndices);

		uint m_MaxVertices;
		uint m_MaxIndices;

		std::vector<Mesh*> m_Meshes;
		Material* m_Material;
		Material* m_MaterialNormals;
		Renderer* m_Renderer;

		GLint m_VertexCount;
		GLint m_IndexCount;

		GLuint m_Vao;
		GLuint m_Vbo;
		GLuint m_Ibo;
		GLuint m_Texture;
	};
};