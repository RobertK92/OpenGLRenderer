#pragma once

namespace E3D
{
	class Entity;

	class World 
	{
		friend class Entity;
	public:
		World();
		~World();

		void Tick(float deltaTime);
		void Destroy();

		Entity* FindEntity(const std::string& name);
		
	private:
		void AddEntity(Entity* entity);
		std::vector<Entity*>::iterator m_EntIter;
		std::vector<Entity*> m_Entities;
	};
};