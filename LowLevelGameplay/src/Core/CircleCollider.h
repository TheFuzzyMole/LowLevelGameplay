#pragma once
#include <Core/Collider.h>
#include <Core/Vector2.h>

namespace LLGP
{
	class BoxCollider;

	class CircleCollider : public Collider
	{
	public:
		CircleCollider(GameObject* owner) : Collider(owner), m_Radius(1.f) {}
		~CircleCollider() = default;

		float GetRadius() { return m_Radius; }
		void SetRadius(float newRadius) { m_Radius = abs(newRadius); }
		//the other and this arguments are the 'wrong' way round becasue in double dispatch we are now looking at the collision from the other side
		Collision* IsColliding(CircleCollider* other) { return Physics::Collision_CircleCircle(other, this); }
		Collision* IsColliding(BoxCollider* other) { return Physics::Collision_AABBCircle(other, this); }

	private:
		float m_Radius;
	};
}
