#pragma once

#include "Graphics/Camera.h"

namespace E3D
{
	class MeshBatch;
	class Material;
	class Mesh;
	class Light;
	class UniformBufferObject;
	class RenderWindow;
	class Transformable;

	class Renderer
	{
		friend class RenderWindow;
		friend class UniformBufferObject;
		friend class MeshBatch;
		friend class Camera;
	public:
		static Renderer* GetInstance()
		{
			static Renderer* renderer;
			if (renderer == nullptr)
				renderer = new Renderer();
			return renderer;
		}
		
		
		MeshBatch* FindBatch(Mesh* mesh, Material* material);
		MeshBatch* CreateBatch(Material* material);
		RenderWindow* GetWindow() const;
		
		std::vector<Transformable*>& GetTransformationStack() const;
		std::vector<Material*>& GetMaterials() const;
		std::vector<Light*>& GetLights() const;
		Camera* GetCamera() const;

		void Update();
		
	private:
		Renderer();
		Renderer(const Renderer&) { };
		const Renderer& operator=(const Renderer&) { };
		void DestroyBatch(MeshBatch* batch);

		void SetWindow(RenderWindow* window);
		void UpdateUniformBuffers();
		
		mutable RenderWindow* m_Window;
		mutable std::vector<UniformBufferObject*> m_UniformBufferObjects;
		mutable std::vector<Transformable*> m_TransformationStack;
		mutable std::vector<MeshBatch*> m_Batches;
		mutable std::vector<Material*> m_Materials;
		mutable std::vector<Light*> m_Lights;
		mutable Camera* m_Camera;
	};
};