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
		Collision* IsColliding(Collider* other) override { return other->IsColliding(this); }
		Collision* IsColliding(CircleCollider* other) override { return Physics::Collision_CircleCircle(other, this); }
		Collision* IsColliding(BoxCollider* other) override  { return Physics::Collision_AABBCircle(other, this); }

		void Serialize(YAML::Emitter& out) override
		{
			out << YAML::Key << "CircleCollider" << YAML::Value << YAML::BeginMap; //CircleCollider
			out << YAML::Key << "UUID" << YAML::Value << uuid;

			out << YAML::Key << "Center" << YAML::Value << m_Center;
			out << YAML::Key << "Radius" << YAML::Value << m_Radius;
			out << YAML::Key << "Restitution" << YAML::Value << m_Restitution;

			out << YAML::EndMap; //CircleCollider
		}

		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override
		{
			if (!node["Center"] || !node["Radius"] || !node["Restitution"]) { return false; }
			m_Center = node["Center"].as<LLGP::Vector2f>();
			m_Radius = node["Radius"].as<float>();
			m_Restitution = node["Restitution"].as<float>();
			return true;
		}
	private:
		float m_Radius;
	};
}
