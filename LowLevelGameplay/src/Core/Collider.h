#pragma once
#include <Core/Component.h>
#include <Core/Vector2.h>
#include <Core/Transform.h>

namespace LLGP
{
	class GameObject;
	class CircleCollider;
	class BoxCollider;
	struct Collision;

	class Collider : public Component
	{
	public:
		Collider(GameObject* owner);
		Collider(GameObject* owner, YAML::Node inData);
		~Collider();

		Vector2<float> GetCenter() { return m_Center; }
		void SetCenter(Vector2<float> newCenter) { m_Center = newCenter; }
		//TODO: make this TransformPosition instead
		Vector2<float> GetPosition();

		virtual Collision* IsColliding(Collider* other) = 0;
		virtual Collision* IsColliding(CircleCollider* other) = 0;
		virtual Collision* IsColliding(BoxCollider* other) = 0;

	protected:
		Vector2<float> m_Center;
	};
}
