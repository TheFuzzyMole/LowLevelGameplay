#pragma once
#include <Core/Components/Collider.h>

namespace LLGP
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(LLGP::GameObject* owner) : Collider(owner), m_Extents(LLGP::Vector2f::one) {}
		
		~BoxCollider() = default;

		LLGP::Vector2f GetExtents();
		void SetExtents(LLGP::Vector2f newExtents);
		LLGP::Vector2f GetBoundsExtents() override;
		
		//the other and this arguments are the 'wrong' way round becasue in double dispatch we are now looking at the collision from the other side
		LLGP::Collision* IsColliding(LLGP::Collider* other) override;
		LLGP::Collision* IsColliding(LLGP::BoxCollider* other) override;
		LLGP::Collision* IsColliding(LLGP::CircleCollider* other) override;
		
		void Serialize(YAML::Emitter& out) override;

		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;
	private:
		LLGP::Vector2f m_Extents;
	};
}
