#pragma once
#include <Core/Collider.h>
#include <Core/Vector2.h>

namespace LLGP
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(GameObject* owner) : Collider(owner), m_Extents(Vector2<float>::one) {}
		~BoxCollider() = default;

		Vector2<float> GetExtents() { return m_Extents; }
		void SetExtents(Vector2<float> newExtents) { m_Extents = Vector2<float>(abs(newExtents.x), abs(newExtents.y)); }
		//the other and this arguments are the 'wrong' way round becasue in double dispatch we are now looking at the collision from the other side
		Collision* IsColliding(BoxCollider* other) { return Physics::Collision_AABBAABB(other, this); }
		Collision* IsColliding(CircleCollider* other) { return Physics::Collision_AABBCircle(other, this); }

	private:
		Vector2<float> m_Extents;
	};
}
