#pragma once

namespace E3D
{
	class Transformable
	{
		friend class Model;
		friend class RenderWindow;
		friend class Camera;
	public:
		Transformable();
		virtual ~Transformable();
		
		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);

		void SetRotation(const glm::quat& rotation);
		void SetRotation(float radians, const glm::vec3& axis);
		void SetRotation(const glm::vec3& eulerAngles);

		void SetScale(float uniformScale);
		void SetScale(float x, float y, float z);
		void SetScale(const glm::vec3& scale);
		
		const glm::vec3& GetForward() const;
		const glm::vec3& GetUp() const;
		const glm::vec3& GetRight() const;
		const glm::vec3& GetPosition() const;
		const glm::quat& GetRotation() const;
		const glm::vec3& GetRotationEuler() const;
		const glm::vec3& GetScale() const;
		const int GetStackIndex() const;
		const Transformable* GetParent() const;
		const std::vector<Transformable*>& GetChildren();

		void LookAt(const Transformable* target);
		void LookAt(const Transformable& target);
		void LookAt(const glm::vec3& target);
		void Rotate(float radians, const glm::vec3& axis);
		void Rotate(const glm::quat& rotation);
		void Translate(float x, float y, float z);
		void Translate(const glm::vec3& translation);
		void Scale(const glm::vec3& scale);
		void Scale(float uniformScale);
		bool HasParent() const;

		const glm::mat4& GetLocalMatrix();
		const glm::mat4& GetGlobalMatrix();
		void UpdateTransformationMatrix(bool forceDirty = false);

		void SetParent(Transformable* parent);
		void UnParent();

	protected:
		void ChangeCoordinateSpace(const glm::mat4& mat);
		virtual void OnTransformed();

		glm::mat4 m_GlobalMatrix;
		glm::mat4 m_LocalMatrix;

		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;
		glm::vec3 m_RotationEuler;
		
		glm::vec3 m_Pivot;
		glm::vec3 m_Forward;
		glm::vec3 m_Right;
		glm::vec3 m_Up;

		Transformable* m_Parent;
		std::vector<Transformable*> m_Children;
		std::vector<Transformable*>::iterator m_ChildIter;

	private:
		void SortTransformationStack();
		bool m_Dirty;
		int m_StackIndex;
	};
};
