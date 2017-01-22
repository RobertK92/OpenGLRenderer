#pragma once

#include "Common/Asset.h"

namespace E3D
{
	class Transformable;
	class Material;
	struct Vertex;
	enum TextureType;

	struct MeshData
	{
		std::vector<Vertex> m_Vertices;
		std::vector<GLuint> m_Indices;
		glm::vec3 m_Pivot;
		glm::vec3 m_LocalPosition;
		glm::quat m_LocalRotation;
		glm::vec3 m_LocalScale;
		Material* m_Material;
		std::string m_ParentName;
		std::string m_Name;
	};

	class ModelData : public Asset
	{
		friend class AssetLoader;
	public:
		std::vector<MeshData>& GetMeshData() const;
		const glm::vec3& GetModelPivot() const;

	private:
		ModelData(const std::string& model);
		const glm::vec3 FindPivot(std::vector<float>& xs, std::vector<float>& ys, std::vector<float>& zs);
		void AssignTextureToMaterial(const char* modelFile, TextureType ttype, const aiScene* scene, aiMesh* mesh, Material* mat);
		std::string GetPathFromFile(const std::string& path);
		std::string GetFileFromPath(const std::string& file);
		void CreateChildren(const std::string& model, const aiScene* scene, aiNode* parentNode);

		mutable std::vector<MeshData> m_MeshData;
		glm::vec3 m_ModelPivot;
	};
};