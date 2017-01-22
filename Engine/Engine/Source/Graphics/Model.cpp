#include "EnginePCH.h"
#include "Graphics/Model.h"
#include "Graphics/ModelData.h"
#include "Common/AssetLoader.h"
#include "Common/Config.h"
#include "Common/World.h"
#include "Common/Game.h"

namespace E3D
{
	Model::Model(const std::string& modelFile)
	{
		m_ModelFile = modelFile;
	}

	Model::~Model()
	{

	}

	void Model::Initialise()
	{
		LoadModel(m_ModelFile.c_str());
	}

	void Model::LoadModel(const char* file)
	{
		m_ModelData = AssetLoader::Load<ModelData>(file); 
		for (auto meshData : m_ModelData->GetMeshData())
			CreateMesh(&meshData);
		CreateTree();
		m_Entity->m_Pivot = m_ModelData->GetModelPivot();
	}

	void Model::CreateMesh(MeshData* meshData)
	{
		Entity* entity = new Entity(meshData->m_Name);
		Mesh* mesh = entity->AddComponent<Mesh>(new Mesh(meshData->m_Vertices, meshData->m_Indices, meshData->m_Material, !Config::UseDynamicBatching()));
		
		mesh->m_ParentName = meshData->m_ParentName;

		mesh->GetEntity()->m_Pivot = meshData->m_Pivot;
		mesh->GetEntity()->SetPosition(meshData->m_LocalPosition);
		mesh->GetEntity()->SetRotation(meshData->m_LocalRotation);
		mesh->GetEntity()->SetScale(meshData->m_LocalScale);
		m_Meshes.insert(std::pair<std::string, Mesh*>(mesh->GetEntity()->GetName(), mesh));
	}
	
	void Model::CreateTree()
	{
		for (auto mesh : m_Meshes)
		{
			if (mesh.second->m_ParentName == "RootNode") 
			{
				ParentTo(mesh.second->GetEntity(), m_Entity);
				continue;
			}

			for (auto m : m_Meshes)
			{
				if (m.second->GetEntity()->GetName() == mesh.second->m_ParentName)
				{
					ParentTo(mesh.second->GetEntity(), m.second->GetEntity());
					break;
				}
			}

			// to make sure mesh is parented to the model, however, this should never happen.
			if (!mesh.second->GetEntity()->HasParent()) { ParentTo(mesh.second->GetEntity(), m_Entity); }
		}
	}

	void Model::ParentTo(Transformable* child, Transformable* parent)
	{
		child->Translate(parent->m_Position);
		child->Rotate(parent->m_Rotation);
		child->Scale(parent->m_Scale - glm::vec3(1));
		child->UpdateTransformationMatrix(true);
		parent->UpdateTransformationMatrix(true);
		child->SetParent(parent);
	}

	std::map<std::string, Mesh*>& Model::GetMeshes() 
	{
		return m_Meshes;
	}

	Mesh* Model::GetMesh(const std::string& meshName)
	{
		if(m_Meshes.find(meshName) != m_Meshes.end())
			return m_Meshes[meshName];
		return nullptr;
	}

	void Model::SetAllMaterials(Material* material)
	{
		for (auto m : m_Meshes)
			m.second->SetMaterial(material);
	}

	Entity* Model::GetRoot() const
	{
		return m_Entity;
	}

};