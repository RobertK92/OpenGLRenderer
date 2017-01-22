#pragma once

#include "Graphics/Mesh.h"
#include "Common/Component.h"

namespace E3D
{
	enum PrimitiveType
	{
		Cube,
		Sphere,
		Plane,
		Cylinder
	};

	struct MeshData;
	class ModelData;

	class Model : public Component
	{
	public:
		virtual ~Model();
		Model(const std::string& modelFile);

		virtual void Initialise() override;

		std::map<std::string, Mesh*>& GetMeshes();
		Mesh* GetMesh(const std::string& meshName);

		void SetAllMaterials(Material* material);
		Entity* GetRoot() const;

	private:
		void LoadModel(const char* file);
		void CreateMesh(MeshData* meshData);
		void CreateTree();
		void ParentTo(Transformable* child, Transformable* parent);

		std::string m_ModelFile;
		std::map<std::string, Mesh*> m_Meshes;
		ModelData* m_ModelData;

	};
};