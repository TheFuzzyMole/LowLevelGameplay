#include "Core\Transform.h"
#include "Core\GameObject.h"
#include "Utils\Maths.h"
#include <cstdlib>
#include <algorithm>

namespace LLGP::Core
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

	Transform::Transform(GameObject* owner, Transform* parent, Vector3d inPos, double inRot, Vector2d inScale)
		: Component(owner), m_Parent(parent), m_LocalPosition(inPos), m_LocalRotation(inRot), m_LocalScale(inScale), m_IsDirty(false)
	{
		if (parent != nullptr) { SetAsDirty(); return; }
		m_Position = inPos;
		m_Rotation = inRot;
		m_Scale = inScale;
	}
	Transform::~Transform()
	{
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
		return m_Children.size() / sizeof(Transform);
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
			SetScale(m_Scale);
			SetRotation(m_Rotation);
			SetPosition(m_Position);
			m_Root = m_Parent->GetRoot();
		}
		else
		{
			SetAsDirty();
		}
	}

	LLGP::Vector3d Transform::GetPosition()
	{
		if (m_IsDirty) CleanTransform();

		return m_Position;
	}
	LLGP::Vector3d Transform::GetLocalPosition()
	{
		return m_LocalPosition;
	}
	LLGP::Vector2d Transform::GetScale()
	{
		if (m_IsDirty) CleanTransform();

		return m_Scale;
	}
	LLGP::Vector2d Transform::GetLocalScale()
	{
		return m_LocalScale;
	}
	double Transform::GetRotation()
	{
		if (m_IsDirty) CleanTransform();

		return m_Rotation;
	}
	double Transform::GetLocalRotation()
	{
		return m_LocalRotation;
	}

	void Transform::SetPosition(LLGP::Vector3d newPosition)
	{
		m_Position = newPosition;

		Vector3d worldOffset = m_Position - m_Parent->GetPosition();
		Vector3d localRight = m_Parent->GetRightVector();
		Vector3d localUp = m_Parent->GetUpVector();
		Vector2d scaleSpace = m_Parent->GetScale();
		m_LocalPosition = ((localRight * Vector3d::Dot(localRight, worldOffset)) /  scaleSpace.x) + ((localUp * Vector3d::Dot(localUp, worldOffset)) / scaleSpace.y) + (Vector3d::forward * worldOffset.z);

		for (Transform* child : m_Children)
		{
			child->SetAsDirty();
		}
	}
	void Transform::ChangePosition(LLGP::Vector3d changeInPosition)
	{
		SetPosition(GetPosition() + changeInPosition);
	}
	void Transform::SetLocalPosition(LLGP::Vector3d newLocalPosition)
	{
		m_LocalPosition = newLocalPosition;

		SetAsDirty();
	}
	void Transform::ChangeLocalPosition(LLGP::Vector3d changeInLocalPosition)
	{
		m_LocalPosition += changeInLocalPosition;

		SetAsDirty();
	}
	void Transform::SetScale(LLGP::Vector2d newScale)
	{
		m_Scale = newScale;
		Vector2d localRight = m_Parent->GetRightVector();
		Vector2d localUp = m_Parent->GetUpVector();
		
		m_LocalScale = (Vector2d::up * Vector2d::Dot(localUp, m_Scale)) + (Vector2d::right * Vector2d::Dot(localRight, m_Scale));

		for (Transform* child : m_Children)
		{
			child->SetAsDirty();
		}
	}
	void Transform::ChangeScale(LLGP::Vector2d changeInScale)
	{
		SetScale(GetScale() + changeInScale);
	}
	void Transform::SetLocalScale(LLGP::Vector2d newLocalScale)
	{
		m_LocalScale = newLocalScale;

		SetAsDirty();
	}
	void Transform::ChangeLocalScale(LLGP::Vector2d changeInLocalScale)
	{
		m_LocalScale += changeInLocalScale;

		SetAsDirty();
	}
	void Transform::SetRotation(double newRotation)
	{
		m_Rotation = newRotation;
		m_LocalRotation = dmod((m_Rotation - m_Parent->GetRotation() + 360.0), 360.0);

		for (Transform* child : m_Children)
		{
			child->SetAsDirty();
		}
	}
	void Transform::ChangeRotation(double changeInRotation)
	{
		SetRotation(
			dmod(
				(GetRotation() + changeInRotation + (360.0 * (abs(changeInRotation) / 360))),
				360.0
			)
		);
	}
	void Transform::SetLocalRotation(double newLocalRotation)
	{
		m_LocalRotation = newLocalRotation;

		SetAsDirty();
	}
	void Transform::ChangeLocalRotation(double changeInLocalRotation)
	{
		SetLocalRotation(
			dmod(
				(m_LocalRotation + changeInLocalRotation + (360.0 * (abs(changeInLocalRotation) / 360))),
				360.0
			)
		);
	}

	void Transform::OwnerActiveChanged(bool newActive)
	{
	}

	Vector3d Transform::GetUpVector()
	{
		return { sin(GetRotation() * DEG2RAD), cos(GetRotation() * DEG2RAD), 0.0};
	}
	Vector3d Transform::GetRightVector()
	{
		return { cos(GetRotation() * DEG2RAD), sin(GetRotation() * DEG2RAD), 0.0};
	}

	//TODO: make this just clean transform and take into account it may be the root
	void Transform::CleanTransform()
	{
		if (!m_IsDirty) return; //already clean

		//update all world space transform component
		if (!m_Parent)
		{
			m_Position = m_LocalPosition;
			m_Rotation = m_LocalRotation;
			m_Scale = m_LocalScale;
			m_Root = this;
		}
		else
		{
			//rotation is easy as it's 1D: take the parent rotation, add local and dmod 360.0
			m_Rotation = dmod(m_Parent->GetRotation() + m_LocalRotation, 360.0);

			//scale also deceptively easy: take parent scale then add x's to x and y's to y
			//works because the local is a projection of the world onto local up and right vectors
			Vector2d parentScale = m_Parent->GetScale();
			m_Scale = (Vector2d::right * ((cos(m_Rotation * DEG2RAD) * m_LocalScale.x) + (sin(m_Rotation * DEG2RAD) * m_LocalScale.y)) * parentScale.x) +
				(Vector2d::up * ((sin(m_Rotation * DEG2RAD) * m_LocalScale.x) + (sin(m_Rotation * DEG2RAD) * m_LocalScale.y)) * parentScale.y);

			//position uses scaled versions of parent up and right vectors * local position
			Vector3d parentWorldRight = m_Parent->GetRightVector();
			parentWorldRight = { parentWorldRight.x * parentScale.x, parentWorldRight.y * parentScale.y, 0.0 };
			Vector3d parentWorldUp = m_Parent->GetUpVector();
			parentWorldUp = { parentWorldUp.x * parentScale.x, parentWorldUp.y * parentScale.y, 0.0 };
			m_Position = m_Parent->GetPosition() + (parentWorldRight * m_LocalPosition.x) + (parentWorldUp * m_LocalPosition.y) + (Vector3d::forward * m_LocalPosition.z);

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
