#include <Core/Components/Collider.h>
#include <Core/GameObject.h>

namespace LLGP
{
	Collider::Collider(GameObject* owner) : Component(owner) { }

	Collider::~Collider()
	{
		Physics::RemoveCollider(this);
	}

    LLGP::Vector2f Collider::GetBoundsExtents()
    {
        return LLGP::Vector2f::zero;
    }

    Vector2<float> Collider::GetPosition()
	{
		return (Vector2<float>)(_GameObject->transform->GetPosition()) + m_Center;
	}

	void Collider::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::Key << "Center" << YAML::Value << m_Center;
		out << YAML::Key << "Restitution" << YAML::Value << m_Restitution;
		out << YAML::Key << "IsTrigger" << YAML::Value << m_IsTrigger;
	}

	bool Collider::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["Center"] || !node["Restitution"] || !node["IsTrigger"]) { return false; }
		m_Center = node["Center"].as<LLGP::Vector2f>();
		m_Restitution = node["Restitution"].as<float>();
		m_IsTrigger = node["IsTrigger"].as<bool>();
		Physics::RegisterCollider(this);
		return true;
	}
}
