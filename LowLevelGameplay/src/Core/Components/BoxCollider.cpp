#include "BoxCollider.h"
#include "Core/GameObject.h"
#include <Core/Maths/Vector2.h>
#include <Core/Physics.h>

namespace LLGP
{
	LLGP::Vector2f BoxCollider::GetExtents()
	{
		return m_Extents;
	}

	void BoxCollider::SetExtents(LLGP::Vector2f newExtents)
	{
		m_Extents = { abs(newExtents.x), abs(newExtents.y) };
	}

	LLGP::Vector2f BoxCollider::GetBoundsExtents()
	{
		LLGP::Vector2f pp, pn, np, nn;
		pp = _GameObject->transform->TransformVector(m_Extents);
		pn = _GameObject->transform->TransformVector({ m_Extents.x, -m_Extents.y });
		np = _GameObject->transform->TransformVector({ -m_Extents.x, m_Extents.y });
		nn = _GameObject->transform->TransformVector(-m_Extents);
		return { fmaxf(fmaxf(pp.x, nn.x), fmaxf(pn.x, np.x)), fmaxf(fmaxf(pp.y, nn.y), fmaxf(pn.y, np.y)) };
	}
	
	LLGP::Collision* BoxCollider::IsColliding(LLGP::Collider* other)
	{
		return other->IsColliding(this);
	}
	LLGP::Collision* BoxCollider::IsColliding(LLGP::BoxCollider* other)
	{
		return LLGP::Physics::Collision_AABBAABB(other, this);
	}
	LLGP::Collision* BoxCollider::IsColliding(LLGP::CircleCollider* other)
	{
		return LLGP::Physics::Collision_AABBCircle(other, this);
	}
	void BoxCollider::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "BoxCollider" << YAML::Value << YAML::BeginMap; //BoxCollider

		LLGP::Collider::Serialize(out);

		out << YAML::Key << "Extents" << YAML::Value << m_Extents;

		out << YAML::EndMap; //BoxCollider
	}
	bool BoxCollider::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["Extents"] || !LLGP::Collider::Deserialize(node, linkRequests)) { return false; }
		m_Extents = node["Extents"].as<LLGP::Vector2f>();
		return true;
	}
}
