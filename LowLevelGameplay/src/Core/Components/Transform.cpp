#include <Core/Components/Transform.h>
#include <Core/GameObject.h>
#include <Core/Maths/Maths.h>
#include <cstdlib>
#include <algorithm>

namespace LLGP
{
	//std::vector<Transform*> Transform::s_DirtyList;

	void Transform::CleanUpDirtyList()
	{
		while(s_DirtyList.size() > 0)
		{
			s_DirtyList[0]->CleanTransform();
		}
		s_DirtyList.clear();
	}

	Transform::Transform(GameObject* owner, Transform* parent, Vector2f inPos)
		: Component(owner), m_Parent(parent), m_LocalPosition(inPos), m_LocalRotation(0.f), m_LocalScale({1,1}), m_IsDirty(false)
	{
		if (parent != nullptr) { SetAsDirty(); return; }
		m_L2WMatrix.FromPRS(inPos, 0.f, LLGP::Vector2f::one);
	}

	Transform* Transform::GetChild(int index)
	{
		if (index < 0 || index >= m_Children.size()) return nullptr;

		return m_Children[index];
	}
	Transform* Transform::GetChild(const std::string& name)
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
	std::vector<LLGP::Transform*>& Transform::GetChildren()
	{
		return m_Children;
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
		CleanTransform();
		m_Parent = parent;
		m_Parent->SetNewChild(this);
		if (worldTransformStays)
		{
			//smash the L2W matrix into local space of the new parent L2W
			LLGP::Mat3f::Decompose(m_L2WMatrix * parent->GetL2WMatrix().Inversed(),
				m_LocalPosition, m_LocalRotation, m_LocalScale);
		}
		m_Root = m_Parent->GetRoot();
		SetAsDirty();
	}

	LLGP::Vector2f Transform::GetPosition()
	{
		if (m_IsDirty) { CleanTransform(); }
		LLGP::Vector2f pos, scale; float rot;
		LLGP::Mat3f::Decompose(m_L2WMatrix, pos, rot, scale);
		return pos;
	}
	const LLGP::Vector2f& Transform::GetLocalPosition()
	{
		return m_LocalPosition;
	}
	float Transform::GetRotation()
	{
		if (m_IsDirty) { CleanTransform(); }
		LLGP::Vector2f pos, scale; float rot;
		LLGP::Mat3f::Decompose(m_L2WMatrix, pos, rot, scale);
		return rot;
	}
	const float& Transform::GetLocalRotation()
	{
		return m_LocalRotation;
	}
	LLGP::Vector2f Transform::GetScale()
	{
		if (m_IsDirty) { CleanTransform(); }
		LLGP::Vector2f pos, scale; float rot;
		LLGP::Mat3f::Decompose(m_L2WMatrix, pos, rot, scale);
		return scale;
	}
	const LLGP::Vector2f& Transform::GetLocalScale()
	{
		return m_LocalScale;
	}

	const LLGP::Mat3f& Transform::GetL2WMatrix()
	{
		return m_L2WMatrix;
	}

#pragma region (Inverse) Transformations
	LLGP::Vector2f Transform::TransformDirection(const LLGP::Vector2f& in)
	{
		//local to world with rotation
		LLGP::Vector2f pos, scale; float rot;
		LLGP::Mat3f::Decompose(m_L2WMatrix, pos, rot, scale);
		return in * LLGP::Mat3f::FromRot(rot);
	}

	LLGP::Vector2f Transform::TransformVector(const LLGP::Vector2f& in)
	{
		//local to world with rotation and scale
		LLGP::Vector2f pos, scale; float rot;
		LLGP::Mat3f::Decompose(m_L2WMatrix, pos, rot, scale);
		return in * LLGP::Mat3f::FromRS(rot, scale);
	}

	LLGP::Vector2f Transform::TransformPoint(const LLGP::Vector2f& in)
	{
		//local to world with all
		return in * m_L2WMatrix;
	}

	//TODO: make these inverse functions better by using local values in matrix multiplied by parent L2W
	LLGP::Vector2f Transform::InverseTransformDirection(const LLGP::Vector2f& in)
	{
		//world to local with rotation
		LLGP::Vector2f pos, scale; float rot;
		LLGP::Mat3f::Decompose(m_L2WMatrix, pos, rot, scale);
		return in * LLGP::Mat3f::FromRot(rot).Inverse();
	}

	LLGP::Vector2f Transform::InverseTransformVector(const LLGP::Vector2f& in)
	{
		//world to local with rotation and scale
		LLGP::Vector2f pos, scale; float rot;
		LLGP::Mat3f::Decompose(m_L2WMatrix, pos, rot, scale);
		return in * LLGP::Mat3f::FromRS(rot, scale).Inverse();
	}

	LLGP::Vector2f Transform::InverseTransformPoint(const LLGP::Vector2f& in)
	{
		//world to local with all
		return in * m_L2WMatrix.Inversed();
	}
#pragma endregion

	void Transform::SetPosition(LLGP::Vector2f newPosition)
	{
		if (!m_Parent)
		{
			m_LocalPosition = newPosition;
		}
		else
		{
			m_LocalPosition = m_Parent->InverseTransformPoint(newPosition);
		}

		//m_LocalPosition = newPosition - (m_Parent ? m_Parent->GetPosition() : LLGP::Vector2f::zero);
		
		SetAsDirty();
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

	void Transform::SetRotation(float newRotation)
	{
		LLGP::Vector2f pos, scale;
		LLGP::Mat3f::Decompose(LLGP::Mat3f::FromRot(newRotation) * m_L2WMatrix.Inversed(),
			pos, m_LocalRotation, scale);
		for (Transform* child : m_Children)
		{
			child->SetAsDirty();
		}
	}
	void Transform::ChangeRotation(float changeInRotation)
	{
		SetRotation(GetRotation() + changeInRotation);
	}
	void Transform::SetLocalRotation(float newLocalRotation)
	{
		m_LocalRotation = newLocalRotation;
		SetAsDirty();
	}
	void Transform::ChangeLocalRotation(float changeInLocalRotation)
	{
		m_LocalRotation += changeInLocalRotation;
		m_LocalRotation = static_cast<float>(dmod((double)m_LocalRotation + (2.0 * PI), 2.0 * PI));
		SetAsDirty();
	}

	void Transform::SetScale(LLGP::Vector2f newScale)
	{
		LLGP::Vector2f pos; float rot;
		LLGP::Mat3f::Decompose(LLGP::Mat3f::FromScale(newScale) * m_L2WMatrix.Inversed(),
			pos, rot, m_LocalScale);
		for (Transform* child : m_Children)
		{
			child->SetAsDirty();
		}
	}
	void Transform::ChangeScale(LLGP::Vector2f changeInScale)
	{
		SetScale(GetScale() + changeInScale);
	}
	void Transform::SetLocalScale(LLGP::Vector2f newLocalScale)
	{
		m_LocalScale = newLocalScale;
		SetAsDirty();
	}
	void Transform::ChangeLocalScale(LLGP::Vector2f changeInLocalScale)
	{
		m_LocalScale += changeInLocalScale;
		SetAsDirty();
	}

	void Transform::OwnerActiveChanged(bool newActive)
	{
	}

	void Transform::CleanTransform()
	{
		if (!m_IsDirty) return; //already clean

		//update all world space transform component
		m_L2WMatrix = LLGP::Mat3f::FromPRS(m_LocalPosition, m_LocalRotation, m_LocalScale);
		if (!m_Parent)
		{
			m_Root = this;
		}
		else
		{
			m_L2WMatrix = m_L2WMatrix * m_Parent->GetL2WMatrix();

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
    void Transform::Serialize(YAML::Emitter& out)
    {
		out << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap; //Transform

		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::Key << "Position" << YAML::Value << GetLocalPosition();
		if (m_Parent)
		{
			out << YAML::Key << "Parent" << YAML::Value << m_Parent->uuid;
		}

		out << YAML::EndMap; //Transform
    }
	bool Transform::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["Position"]) { return false; }
		m_LocalPosition = node["Position"].as<LLGP::Vector2f>();

		if (node["Parent"])
		{
			linkRequests.emplace_back(std::bind(
				[](LLGP::Transform* context, LLGP::Component* parent)
					{context->SetParent(dynamic_cast<LLGP::Transform*>(parent), false); },
					this, std::placeholders::_1),
				node["Parent"].as<uint64_t>());
		}
		else
		{
			m_Parent = nullptr;
		}
		SetAsDirty();
		return true;
	}
}
