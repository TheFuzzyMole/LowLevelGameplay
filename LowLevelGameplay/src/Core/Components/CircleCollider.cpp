#include "CircleCollider.h"

#include <Core/Physics.h>

namespace LLGP
{

	float CircleCollider::GetRadius()
	{
		return m_Radius;
	}

	void CircleCollider::SetRadius(float newRadius)
	{
		m_Radius = abs(newRadius);
	}

	LLGP::Vector2f CircleCollider::GetBoundsExtents()
	{
		return { m_Radius, m_Radius };
	}

	LLGP::Collision* CircleCollider::IsColliding(LLGP::Collider* other)
	{
		return other->IsColliding(this);
	}

	LLGP::Collision* CircleCollider::IsColliding(LLGP::CircleCollider* other)
	{
		return LLGP::Physics::Collision_CircleCircle(other, this);
	}

	LLGP::Collision* CircleCollider::IsColliding(LLGP::BoxCollider* other)
	{
		return LLGP::Physics::Collision_AABBCircle(other, this);
	}

	void CircleCollider::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "CircleCollider" << YAML::Value << YAML::BeginMap; //CircleCollider

		Collider::Serialize(out);

		out << YAML::Key << "Radius" << YAML::Value << m_Radius;

		out << YAML::EndMap; //CircleCollider
	}

	bool CircleCollider::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["Radius"] || !Collider::Deserialize(node, linkRequests)) { return false; }
		m_Radius = node["Radius"].as<float>();
		return true;
	}
}
