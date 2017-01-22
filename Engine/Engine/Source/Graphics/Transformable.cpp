#include "EnginePCH.h"
#include "Common/Transformable.h"
#include "Common/Entity.h"
#include "Graphics/Renderer.h"

namespace E3D  
{
	Transformable::Transformable()
	{
		m_Position		 = glm::vec3();
		m_Rotation		 = glm::quat();
		m_Scale			 = glm::vec3(1);
		m_Pivot			 = glm::vec3();
		m_Forward		 = glm::vec3(0, 0, -1);
		m_Up			 = glm::vec3(0, -1, 0);
		m_Right			 = glm::vec3(-1, 0, 0);
		m_GlobalMatrix	 = glm::mat4(1);
		m_StackIndex	 = 0;
		m_Dirty			 = true;
		m_Parent		 = nullptr;
		Renderer::GetInstance()->GetTransformationStack().push_back(this);
	}

	E3D::Transformable::~Transformable()
	{
		Renderer::GetInstance()->GetTransformationStack().erase(
			std::remove(
				Renderer::GetInstance()->GetTransformationStack().begin(),
				Renderer::GetInstance()->GetTransformationStack().end(), this));
	}

	//////////////////////////////////////////////////////////////////////////
	// getters
	//////////////////////////////////////////////////////////////////////////
	const glm::vec3& Transformable::GetPosition()const
	{
		return m_Position;
	}

	const glm::quat& Transformable::GetRotation()const
	{
		return m_Rotation;
	}

	const glm::vec3& Transformable::GetRotationEuler()const
	{
		return m_RotationEuler;
	}

	const glm::vec3& Transformable::GetScale()const
	{
		return m_Scale;
	}

	const glm::vec3& Transformable::GetForward() const
	{
		return m_Forward;
	}

	const glm::vec3& Transformable::GetUp() const
	{
		return m_Up;
	}

	const glm::vec3& Transformable::GetRight() const
	{
		return m_Right;
	}

	const int Transformable::GetStackIndex() const
	{
		return m_StackIndex;
	}

	const Transformable* Transformable::GetParent() const
	{
		return m_Parent;
	}

	const std::vector<Transformable*>& Transformable::GetChildren() 
	{
		return m_Children;
	}

	const glm::mat4& Transformable::GetLocalMatrix()
	{
		return m_LocalMatrix;
	}

	const glm::mat4& Transformable::GetGlobalMatrix()
	{
		if(HasParent())
			return m_GlobalMatrix;
		return m_LocalMatrix;
	}
	//////////////////////////////////////////////////////////////////////////

	void Transformable::UpdateTransformationMatrix(bool forceDirty /*= false*/)
	{
		if (m_Dirty || forceDirty)
		{
			glm::mat4 rotMat = glm::mat4_cast(m_Rotation);

			m_LocalMatrix = glm::mat4(
				glm::translate(m_Position) *
				glm::translate((m_Pivot)) *
				glm::scale(m_Scale) *
				glm::transpose(rotMat) *
				glm::translate(-(m_Pivot))
				);
			
			if (HasParent()) 
			{
				m_GlobalMatrix = m_Parent->GetGlobalMatrix() * GetLocalMatrix();
			}

			// calculate forward, right and up vectors
			static glm::quat quaternion = glm::quat();
			glm::decompose(m_GlobalMatrix, glm::vec3(), quaternion, glm::vec3(), glm::vec3(), glm::vec4());
			rotMat = glm::mat4_cast(quaternion);

			m_Right.x	= rotMat[0][0];
			m_Right.y	= rotMat[1][0];
			m_Right.z	= rotMat[2][0];

			m_Up.x		= rotMat[0][1];
			m_Up.y		= rotMat[1][1];
			m_Up.z		= rotMat[2][1];

			m_Forward.x = rotMat[0][2];
			m_Forward.y = rotMat[1][2];
			m_Forward.z = rotMat[2][2];

			for (m_ChildIter = m_Children.begin(); m_ChildIter != m_Children.end(); m_ChildIter++) 
				(*m_ChildIter)->m_Dirty = true;

			m_Dirty = false;
			OnTransformed();
		}
	}
	
	struct __comparer {
		bool operator() (Transformable* i, Transformable* j) { return (i->GetStackIndex() < j->GetStackIndex()); }
	} __stackIndexCompare;

	void Transformable::SortTransformationStack()
	{
		// OPTIMIZE
		std::sort(
			Renderer::GetInstance()->GetTransformationStack().begin(), 
			Renderer::GetInstance()->GetTransformationStack().end(), 
			__stackIndexCompare);
	}

	void Transformable::SetParent(Transformable* parent)
	{
		if (parent == m_Parent || parent == this || !parent)
			return;

		if (parent->HasParent())
		{
			if (parent->m_Parent == this)
			{
				E3DPrint("Cannot parent an object to one of its own children (unparent first)");
				return;
			}
		}

		UpdateTransformationMatrix();
		parent->UpdateTransformationMatrix();

		bool wasChild = HasParent();
		UnParent();
		m_Parent = parent;
		
		if (HasParent()) 
		{
			m_Parent->m_Children.push_back(this);

			glm::mat4 res = glm::mat4(1);
			glm::mat4 parentMat = m_Parent->GetGlobalMatrix();
			glm::mat4 myMat = m_GlobalMatrix;

			if (!wasChild)
				myMat = m_LocalMatrix;
		
			parentMat = glm::inverse(parentMat);
			res = parentMat * myMat;
			
			ChangeCoordinateSpace(res);
			m_StackIndex = m_Parent->m_StackIndex + 1;
			SortTransformationStack();
		}

		m_Dirty = true;
	}

	void Transformable::UnParent()
	{
		if (HasParent()) 
		{
			m_Parent->m_Children.erase(std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this), m_Parent->m_Children.end());
			ChangeCoordinateSpace(m_GlobalMatrix);
			m_Parent = nullptr;
			m_Dirty = true;
			m_StackIndex = 0;
			SortTransformationStack();
		}
	}

	void Transformable::ChangeCoordinateSpace(const glm::mat4& mat)
	{
		glm::vec3 pos;
		glm::quat rot;
		glm::vec3 scal;
		glm::decompose(mat, scal, rot, pos, glm::vec3(), glm::vec4());
		m_Position = pos;
		m_Rotation = rot;
		m_Scale = scal;

	}

	void Transformable::OnTransformed() { }

	//////////////////////////////////////////////////////////////////////////
	// helper functions
	//////////////////////////////////////////////////////////////////////////
	void Transformable::Rotate(float radians, const glm::vec3& axis)
	{
		if (axis == glm::vec3()) return;
		glm::quat r = glm::quat(glm::rotate(radians, axis));
		SetRotation(m_Rotation * r);
	}

	void Transformable::Rotate(const glm::quat& rotation)
	{
		SetRotation(m_Rotation * rotation);
	}

	void Transformable::Translate(float x, float y, float z)
	{
		SetPosition(m_Position + glm::vec3(x, y, z));
	}

	void Transformable::Translate(const glm::vec3& translation)
	{
		SetPosition(m_Position + translation);
	}

	void Transformable::Scale(const glm::vec3& scale)
	{
		SetScale(m_Scale + scale);
	}

	void Transformable::Scale(float uniformScale)
	{
		SetScale(m_Scale + (glm::vec3(1) * uniformScale));
	}

	bool Transformable::HasParent() const
	{
		return m_Parent != nullptr;
	}

	void Transformable::LookAt(const Transformable* target)
	{
		LookAt(target->m_Position);
	}

	void Transformable::LookAt(const Transformable& target)
	{
		LookAt(target.m_Position);
	}

	// !
	void Transformable::LookAt(const glm::vec3& target)
	{
		 glm::mat4 look = glm::lookAt(m_Position, target, glm::vec3(0, 1, 0));
		 SetRotation(glm::quat(look));
	}

	//////////////////////////////////////////////////////////////////////////
	// set rotation
	//////////////////////////////////////////////////////////////////////////
	void Transformable::SetRotation(float radians, const glm::vec3& axis)
	{
		if (axis == glm::vec3()) return;
		SetRotation(glm::quat(glm::rotate(radians, axis)));
	}

	void Transformable::SetRotation(const glm::vec3& eulerAngles)
	{
		SetRotation(glm::quat(eulerAngles));
	}

	// !
	void Transformable::SetRotation(const glm::quat& rotation)
	{
		m_Rotation = rotation;
		m_RotationEuler = glm::eulerAngles(m_Rotation);
		m_Dirty = true;
	}

	//////////////////////////////////////////////////////////////////////////
	// set position
	//////////////////////////////////////////////////////////////////////////
	void Transformable::SetPosition(const glm::vec3& position)
	{
		SetPosition(position.x, position.y, position.z);
	}

	// !
	void Transformable::SetPosition(float x, float y, float z)
	{
		m_Position = glm::vec3(x, y, z);
		m_Dirty = true;
	}

	//////////////////////////////////////////////////////////////////////////
	// set scale
	//////////////////////////////////////////////////////////////////////////
	void Transformable::SetScale(const glm::vec3& scale)
	{
		SetScale(scale.x, scale.y, scale.z);
	}

	void Transformable::SetScale(float uniformScale)
	{
		SetScale(uniformScale, uniformScale, uniformScale);
	}

	// !
	void Transformable::SetScale(float x, float y, float z)
	{
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
		m_Dirty = true;
	}

	//////////////////////////////////////////////////////////////////////////
};


