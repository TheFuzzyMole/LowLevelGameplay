#include <Core/Collider.h>
#include <Core/GameObject.h>

namespace LLGP
{
	Collider::Collider(GameObject* owner) : Component(owner)
	{
		Physics::RegisterCollider(this);
	}

	Collider::~Collider()
	{
		Physics::RemoveCollider(this);
	}

	Vector2<float> Collider::GetPosition()
	{
		return (Vector2<float>)(_GameObject->transform->GetPosition()) + m_Center;
	}
}
