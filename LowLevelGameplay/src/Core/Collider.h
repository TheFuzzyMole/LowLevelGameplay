#pragma once
#include <Core/Component.h>
#include <Core/Vector2.h>
#include <Core/Transform.h>

namespace LLGP
{
	class GameObject;
	struct Collision;

	class Collider : public Component
	{
	public:
		Collider(GameObject* owner);
		~Collider();

		Vector2<float> GetCenter() { return m_Center; }
		void SetCenter(Vector2<float> newCenter) { m_Center = newCenter; }
		//TODO: make this TransformPosition instead
		Vector2<float> GetPosition();

		virtual Collision* IsColliding(Collider* other) { return other->IsColliding(this); }

	protected:
		Vector2<float> m_Center;
	};
}
