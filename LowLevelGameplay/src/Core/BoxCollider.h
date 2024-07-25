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
		Collision* IsColliding(Collider* other) override { return other->IsColliding(this); }
		Collision* IsColliding(BoxCollider* other) override { return Physics::Collision_AABBAABB(other, this); }
		Collision* IsColliding(CircleCollider* other) override { return Physics::Collision_AABBCircle(other, this); }
		
		void Serialize(YAML::Emitter& out) override
		{
			out << YAML::Key << "BoxCollider" << YAML::Value << YAML::BeginMap; //BoxCollider
			out << YAML::Key << "UUID" << YAML::Value << uuid;

			out << YAML::Key << "Center" << YAML::Value << m_Center;
			out << YAML::Key << "Extents" << YAML::Value << m_Extents;
			out << YAML::Key << "Restitution" << YAML::Value << m_Restitution;

			out << YAML::EndMap; //BoxCollider
		}

		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override
		{
			if (!node["Center"] || !node["Extents"] || !node["Restitution"]) { return false; }
			m_Center = node["Center"].as<LLGP::Vector2f>();
			m_Extents = node["Extents"].as<LLGP::Vector2f>();
			m_Restitution = node["Restitution"].as<float>();
			return true;
		}
	private:
		Vector2<float> m_Extents;
	};
}
