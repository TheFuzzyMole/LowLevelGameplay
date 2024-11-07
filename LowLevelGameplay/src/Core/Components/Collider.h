#pragma once
#include <Core/Components/Component.h>
#include <Core/Maths/Vector2.h>
#include <Core/Components/Transform.h>

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
		~Collider();

		Vector2<float> GetCenter() { return m_Center; }
		void SetCenter(Vector2<float> newCenter) { m_Center = newCenter; }
		float GetRestitution() { return m_Restitution; }
		void SetRestitution(float _restitution) { m_Restitution = _restitution; }
		bool GetIsTrigger() { return m_IsTrigger; }
		void SetIsTrigger(bool _isTrigger) { m_IsTrigger = _isTrigger; }
		virtual LLGP::Vector2f GetBoundsExtents();
		Vector2<float> GetPosition();

		virtual Collision* IsColliding(Collider* other) = 0;
		virtual Collision* IsColliding(CircleCollider* other) = 0;
		virtual Collision* IsColliding(BoxCollider* other) = 0;

	protected:
		Vector2<float> m_Center;
		float m_Restitution;
		bool m_IsTrigger;

		virtual void Serialize(YAML::Emitter& out) override;
		virtual bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;
	};
}
