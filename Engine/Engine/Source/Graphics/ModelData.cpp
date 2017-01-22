#include "EnginePCH.h"
#include "Graphics/ModelData.h"
#include "Graphics/Vertex.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"

#include "Common/AssetLoader.h"

namespace E3D
{
	ModelData::ModelData(const std::string& model)
		: Asset(model)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(model,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords);

		if (!scene)
		{
			E3DShowError(importer.GetErrorString());
		}
		else
		{
			CreateChildren(model, scene, scene->mRootNode);
			
			std::vector<float> sortX, sortY, sortZ;
			sortX.resize(m_MeshData.size());
			sortY.resize(m_MeshData.size());
			sortZ.resize(m_MeshData.size());

			for (uint i = 0; i < m_MeshData.size(); i++)
			{
				sortX[i] = m_MeshData[i].m_Pivot.x;
				sortY[i] = m_MeshData[i].m_Pivot.y;
				sortZ[i] = m_MeshData[i].m_Pivot.z;
			}

			m_ModelPivot = FindPivot(sortX, sortY, sortZ);
		}
	}

	void ModelData::CreateChildren(const std::string& model, const aiScene* scene, aiNode* parentNode)
	{
		for (uint g = 0; g < parentNode->mNumChildren; g++)
		{
			aiNode* child = parentNode->mChildren[g];

			for (uint i = 0; i < child->mNumMeshes; i++)
			{
				MeshData data;

				aiMaterial* mat = scene->mMaterials[scene->mMeshes[child->mMeshes[i]]->mMaterialIndex];
				uint totalCount = 0;
				totalCount += mat->GetTextureCount(aiTextureType_AMBIENT);
				totalCount += mat->GetTextureCount(aiTextureType_DIFFUSE);
				totalCount += mat->GetTextureCount(aiTextureType_DISPLACEMENT);
				totalCount += mat->GetTextureCount(aiTextureType_EMISSIVE);
				totalCount += mat->GetTextureCount(aiTextureType_HEIGHT);
				totalCount += mat->GetTextureCount(aiTextureType_LIGHTMAP);
				totalCount += mat->GetTextureCount(aiTextureType_NORMALS);
				totalCount += mat->GetTextureCount(aiTextureType_OPACITY);
				totalCount += mat->GetTextureCount(aiTextureType_REFLECTION);
				totalCount += mat->GetTextureCount(aiTextureType_SHININESS);
				totalCount += mat->GetTextureCount(aiTextureType_SPECULAR);
				totalCount += mat->GetTextureCount(aiTextureType_UNKNOWN);
				totalCount += mat->GetTextureCount(_aiTextureType_Force32Bit);

				if (totalCount == 0)
				{
					data.m_Material = Material::Default();
				}
				else
				{
					aiString* aiTex = new aiString();
					mat->GetTexture(aiTextureType_DIFFUSE, 0, aiTex);
					std::string texName = GetFileFromPath(aiTex->C_Str());
					delete aiTex;

					data.m_Material = AssetLoader::Load<Material>(texName);
					AssignTextureToMaterial(model.c_str(), TextureType::Diffuse, scene, scene->mMeshes[child->mMeshes[i]], data.m_Material);
					AssignTextureToMaterial(model.c_str(), TextureType::Specular, scene, scene->mMeshes[child->mMeshes[i]], data.m_Material);
				}
				//TODO: add more types of texture maps 
				
				//////////////////////////////////////////////////////////////////////////

				std::vector<float> sortX, sortY, sortZ;
				
				sortX.resize(scene->mMeshes[child->mMeshes[i]]->mNumVertices);
				sortY.resize(scene->mMeshes[child->mMeshes[i]]->mNumVertices);
				sortZ.resize(scene->mMeshes[child->mMeshes[i]]->mNumVertices);

				// vertices
				for (uint j = 0; j < scene->mMeshes[child->mMeshes[i]]->mNumVertices; j++)
				{
					Vertex vert;

					// position					
					vert.m_Pos.x = scene->mMeshes[child->mMeshes[i]]->mVertices[j].x; sortX[j] = vert.m_Pos.x;
					vert.m_Pos.y = scene->mMeshes[child->mMeshes[i]]->mVertices[j].y; sortY[j] = vert.m_Pos.y;
					vert.m_Pos.z = scene->mMeshes[child->mMeshes[i]]->mVertices[j].z; sortZ[j] = vert.m_Pos.z;

					// color
					vert.m_Color = glm::vec3(1.0f, 1.0f, 1.0f);

					// texture coordinates
					aiVector3D uvs =
						scene->mMeshes[child->mMeshes[i]]->HasTextureCoords(0) ?
						(scene->mMeshes[child->mMeshes[i]]->mTextureCoords[0][j]) :
						aiVector3D(0, 0, 0);
					vert.m_TexCord = glm::vec2(uvs.x, uvs.y);

					// normals
					aiVector3D normal =
						scene->mMeshes[child->mMeshes[i]]->HasNormals() ?
						scene->mMeshes[child->mMeshes[i]]->mNormals[j] :
						aiVector3D(0, 0, 0);
					vert.m_Normal = glm::vec3(normal.x, normal.y, normal.z);

					// add vertex
					data.m_Vertices.push_back(vert);
				}

				// indices
				for (uint j = 0; j < scene->mMeshes[child->mMeshes[i]]->mNumFaces; j++)
				{
					for (uint k = 0; k < scene->mMeshes[child->mMeshes[i]]->mFaces[j].mNumIndices; k++)
					{
						data.m_Indices.push_back((GLuint)scene->mMeshes[child->mMeshes[i]]->mFaces[j].mIndices[k]);
					}
				}

				data.m_Pivot = FindPivot(sortX, sortY, sortZ);
				
				aiVector3D pos;
				aiVector3D scale;
				aiQuaternion rot;
				child->mTransformation.Decompose(scale, rot, pos);
				data.m_LocalPosition = glm::vec3(pos.x, pos.y, pos.z);
				data.m_LocalRotation = glm::quat(glm::transpose(glm::mat4_cast(glm::quat(rot.w, rot.x, rot.y, rot.z))));
				data.m_LocalScale = glm::vec3(scale.x, scale.y, scale.z);
				data.m_Name = child->mName.data;
				data.m_Name.erase(0, 2);

				if (child->mParent != NULL) 
				{
					data.m_ParentName = child->mParent->mName.data;
					data.m_ParentName.erase(0, 2);
				}
				else
					data.m_ParentName = "RootNode";

				m_MeshData.push_back(data);
			}

			if(child->mNumChildren > 0)
				CreateChildren(model, scene, child);
		}
	}

	void ModelData::AssignTextureToMaterial(const char* modelFile, TextureType ttype, const aiScene* scene, aiMesh* mesh, Material* mat)
	{
		aiTextureType aittype = aiTextureType_DIFFUSE;
		//TODO: add more types of texture maps 
		switch (ttype)
		{
		case E3D::Diffuse:
			aittype = aiTextureType_DIFFUSE;
			break;
		case E3D::Specular:
			aittype = aiTextureType_SPECULAR;
			break;
		default:
			break;
		}

		aiString* aiTex = new aiString();
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aittype, 0, aiTex);

		std::string texName = GetFileFromPath(aiTex->C_Str());
		std::string path = GetPathFromFile(modelFile);


		Texture* texture = nullptr;
		if (texName.size() > 0)
		{
			std::string d = (path + "/" + texName);
			std::ifstream file(d);
			if (file.good()) 
			{
				file.close();
				texture = AssetLoader::Load<Texture>(d);
			}
			else
			{
				file.close();
				// TODO enumerate folders and search recursive up

			}
		}
		
		if (texture)
			mat->SetTexture(texture, ttype);
		delete aiTex;
	}

	std::string ModelData::GetFileFromPath(const std::string& file)
	{
		std::string newTex = "";
		for (int i = file.size() - 1; i >= 0; i--)
		{
			if (file[i] == '/' || file[i] == '\\')
			{
				break;
			}
			newTex += file[i];
		}

		newTex = std::string(newTex.rbegin(), newTex.rend());
		return newTex;
	}

	std::string ModelData::GetPathFromFile(const std::string& path)
	{
		std::string newTex = "";
		bool passed = false;
		for (int i = path.size() - 1; i >= 0; i--)
		{
			if (passed)
				newTex += path[i];
			if (path[i] == '/' || path[i] == '\\')
			{
				passed = true;
			}
		}

		newTex = std::string(newTex.rbegin(), newTex.rend());
		return newTex;
	}

	const glm::vec3 ModelData::FindPivot(std::vector<float>& xs, std::vector<float>& ys, std::vector<float>& zs)
	{
		// OPTIMIZE
		std::sort(xs.begin(), xs.end());
		std::sort(ys.begin(), ys.end());
		std::sort(zs.begin(), zs.end());
		float pivX = (xs[0] + xs[xs.size() - 1]) * 0.5f;
		float pivY = (ys[0] + ys[ys.size() - 1]) * 0.5f;
		float pivZ = (zs[0] + zs[zs.size() - 1]) * 0.5f;
		return glm::vec3(pivX, pivY, pivZ);
	}

	std::vector<MeshData>& ModelData::GetMeshData() const
	{
		return m_MeshData;
	}

	const glm::vec3& ModelData::GetModelPivot() const
	{
		return m_ModelPivot;
	}

};