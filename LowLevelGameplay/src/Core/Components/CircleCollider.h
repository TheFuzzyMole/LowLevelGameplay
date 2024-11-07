#pragma once
#include <Core/Components/Collider.h>

namespace LLGP
{
	class BoxCollider;

	class CircleCollider : public Collider
	{
	public:
		CircleCollider(GameObject* owner) : Collider(owner), m_Radius(1.f) {}
		
		~CircleCollider() = default;

		float GetRadius();
		void SetRadius(float newRadius);
		LLGP::Vector2f GetBoundsExtents() override;

		//the other and this arguments are the 'wrong' way round becasue in double dispatch we are now looking at the collision from the other side
		LLGP::Collision* IsColliding(LLGP::Collider* other) override;
		LLGP::Collision* IsColliding(LLGP::CircleCollider* other) override;
		LLGP::Collision* IsColliding(LLGP::BoxCollider* other) override;

		void Serialize(YAML::Emitter& out) override;

		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;
	private:
		float m_Radius;
	};
}
