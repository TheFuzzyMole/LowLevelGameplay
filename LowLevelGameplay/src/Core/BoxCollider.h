#pragma once
#include <Core/Collider.h>
#include <Core/Vector2.h>

namespace LLGP
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(GameObject* owner) : Collider(owner), m_Extents(Vector2<float>::one) {}
		BoxCollider(GameObject* owner, YAML::Node inData) : Collider(owner, inData)
		{
			if (!Deserialize(inData)) { std::cout << "Error Deserializing BoxCollider: " << uuid << std::endl; }
		}
		~BoxCollider() = default;

		Vector2<float> GetExtents() { return m_Extents; }
		void SetExtents(Vector2<float> newExtents) { m_Extents = Vector2<float>(abs(newExtents.x), abs(newExtents.y)); }
		//the other and this arguments are the 'wrong' way round becasue in double dispatch we are now looking at the collision from the other side
		Collision* IsColliding(BoxCollider* other) { return Physics::Collision_AABBAABB(other, this); }
		Collision* IsColliding(CircleCollider* other) { return Physics::Collision_AABBCircle(other, this); }
		
		void Serialize(YAML::Emitter& out) override
		{
			out << YAML::Key << "BoxCollider";
			out << YAML::BeginMap; //BoxCollider
			out << YAML::Key << "UUID" << YAML::Value << uuid;

			out << YAML::Key << "Center" << YAML::Value << m_Center;
			out << YAML::Key << "Extents" << YAML::Value << m_Extents;

			out << YAML::EndMap; //BoxCollider
		}

		bool Deserialize(YAML::Node node) override
		{
			if (!node["Center"] || !node["Extents"]) { return false; }
			m_Center = node["Center"].as<LLGP::Vector2f>();
			m_Extents = node["Extents"].as<LLGP::Vector2f>();
			return false;
		}
	private:
		Vector2<float> m_Extents;
	};
}
