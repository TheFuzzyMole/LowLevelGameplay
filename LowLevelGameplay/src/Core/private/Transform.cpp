#include "Core\Transform.h"
#include "Core\GameObject.h"
#include "Utils\Maths.h"
#include <cstdlib>
#include <algorithm>

namespace LLGP
{
	std::vector<Transform*> Transform::s_DirtyList;

	void Transform::CleanUpDirtyList()
	{
		while(s_DirtyList.size() > 0)
		{
			s_DirtyList[0]->CleanTransform();
		}
		s_DirtyList.clear();
	}

	Transform::Transform(GameObject* owner, Transform* parent, Vector2f inPos)
		: Component(owner), m_Parent(parent), m_LocalPosition(inPos), m_IsDirty(false)
	{
		if (parent != nullptr) { SetAsDirty(); return; }
		m_Position = inPos;
	}

	Transform* Transform::GetChild(int index)
	{
		if (index < 0 || index >= m_Children.size()) return nullptr;

		return m_Children[index];
	}
	Transform* Transform::GetChild(std::string name)
	{
		for (int i = 0; i < m_Children.size(); i++)
		{
			if (m_Children[i]->GetGameObject()->GetName() == name)
			{
				return m_Children[i];
			}
		}
		return nullptr;
	}
	std::vector<Transform> Transform::GetChildren()
	{
		return std::vector<Transform>();
	}
	int Transform::GetChildCount()
	{
		return (int)m_Children.size();
	}
	/// <summary>
	/// Returns the index of the child in the parent transforms child vector. Returns -1 if child is not found;
	/// </summary>
	int Transform::GetIndexOfChild(Transform* child)
	{
		for (int childIndex = 0; childIndex < m_Children.size(); childIndex++)
		{
			if (m_Children[childIndex] == child)
			{
				return childIndex;
			}
		}
		return -1;
	}

	void Transform::SetNewChild(Transform* newChild)
	{
		if (GetIndexOfChild(newChild) >= 0) return;

		m_Children.push_back(newChild);
	}

	void Transform::RemoveChild(Transform* oldChild)
	{
		if (GetIndexOfChild(oldChild) == -1) return;

		m_Children.erase(std::find(m_Children.begin(), m_Children.end(), oldChild));
	}

	Transform* Transform::GetRoot()
	{
		if (m_IsDirty) CleanTransform();

		return m_Root;
	}

	Transform* Transform::GetParent()
	{
		return m_Parent;
	}
	int Transform::GetSiblingIndex()
	{
		return m_Parent->GetIndexOfChild(this);
	}

	void Transform::SetParent(Transform* parent, bool worldTransformStays)
	{
		if (m_Parent == parent) return;
		m_Parent = parent;
		m_Parent->SetNewChild(this);
		if (worldTransformStays)
		{
			SetPosition(m_Position);
			m_Root = m_Parent->GetRoot();
		}
		else
		{
			SetAsDirty();
		}
	}

	LLGP::Vector2f Transform::GetPosition()
	{
		if (m_IsDirty) CleanTransform();

		return m_Position;
	}
	LLGP::Vector2f Transform::GetLocalPosition()
	{
		return m_LocalPosition;
	}

	void Transform::SetPosition(LLGP::Vector2f newPosition)
	{
		m_Position = newPosition;

		m_LocalPosition = m_Position - (m_Parent ? m_Parent->GetPosition(): LLGP::Vector2f::zero);
		
		for (Transform* child : m_Children)
		{
			child->SetAsDirty();
		}
	}
	void Transform::ChangePosition(LLGP::Vector2f changeInPosition)
	{
		SetPosition(GetPosition() + changeInPosition);
	}
	void Transform::SetLocalPosition(LLGP::Vector2f newLocalPosition)
	{
		m_LocalPosition = newLocalPosition;

		SetAsDirty();
	}
	void Transform::ChangeLocalPosition(LLGP::Vector2f changeInLocalPosition)
	{
		m_LocalPosition += changeInLocalPosition;

		SetAsDirty();
	}

	void Transform::OwnerActiveChanged(bool newActive)
	{
	}

	void Transform::CleanTransform()
	{
		if (!m_IsDirty) return; //already clean

		//update all world space transform component
		if (!m_Parent)
		{
			m_Position = m_LocalPosition;
			m_Root = this;
		}
		else
		{
			m_Position = m_Parent->GetPosition() + m_LocalPosition;

			//root is a recursive call up the chain
			m_Root = m_Parent->GetRoot();
		}

		//remove this transform from the dirty list
		s_DirtyList.erase(std::find(s_DirtyList.begin(), s_DirtyList.end(), this));
		//clear dirty flag
		m_IsDirty = false;
	}
	void Transform::SetAsDirty()
	{
		if (m_IsDirty) return;
		m_IsDirty = true;
		Transform::s_DirtyList.push_back(this);
		for (Transform* child : m_Children)
		{
			child->SetAsDirty();
		}
	}
}
