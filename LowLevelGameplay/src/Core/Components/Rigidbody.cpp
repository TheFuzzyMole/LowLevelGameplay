#include <Core/Components/Rigidbody.h>
#include <Core/Physics.h>
#include <Core/Commons.h>
#include <Core/GameObject.h>

namespace LLGP
{
	Rigidbody::Rigidbody(GameObject* owner)
		: Component(owner), Velocity(LLGP::Vector2f::zero), m_AccumulatedForce(LLGP::Vector2f::zero),
		Mass(1.f), IsKinematic(false), HasGravity(true)
	{
		Physics::RegisterRigidbody(this);
		Physics::OnStepPhysics.AddListener(this, std::bind(&Rigidbody::ApplyAccumulatedForce, this));
	}

	Rigidbody::~Rigidbody()
	{
		Physics::RemoveRigidbody(this);
		Physics::OnStepPhysics.RemoveListener(this, std::bind(&Rigidbody::ApplyAccumulatedForce, this));
	}

	void Rigidbody::AddForce(LLGP::Vector2f force, LLGP::ForceMode mode)
	{
		switch (mode)
		{
		case LLGP::Force:
			m_AccumulatedForce += (force / Mass) * FIXED_FRAMERATE;
			break;
		case LLGP::Acceleration:
			m_AccumulatedForce += force * FIXED_FRAMERATE;
			break;
		case LLGP::Impulse:
			m_AccumulatedForce += (force / Mass);
			break;
		case LLGP::VelocityChange:
			m_AccumulatedForce += force;
			break;
		}
	}

	void Rigidbody::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "Rigidbody";
		out << YAML::BeginMap; //Rigidbody
		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::Key << "Mass" << YAML::Value << Mass;
		out << YAML::Key << "IsKinematic" << YAML::Value << IsKinematic;
		out << YAML::Key << "HasGravity" << YAML::Value << HasGravity;

		out << YAML::EndMap; //Rigidbody
	}

	bool Rigidbody::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["Mass"] || !node["IsKinematic"] || !node["HasGravity"]) { return false; }
		Mass = node["Mass"].as<float>();
		IsKinematic = node["IsKinematic"].as<bool>();
		HasGravity = node["HasGravity"].as<bool>();
		return true;
	}

	void Rigidbody::ApplyAccumulatedForce()
	{
		if (IsKinematic) { m_AccumulatedForce = LLGP::Vector2f::zero; return; }
		if (HasGravity) { m_AccumulatedForce += Physics::Gravity * FIXED_FRAMERATE / PHYSICS_SOLVER_ITERATIONS; }
		Velocity += m_AccumulatedForce;
		if (Velocity.GetSqrMagnitude() < EPSILON) { return; }
		_GameObject->transform->ChangePosition(Velocity * FIXED_FRAMERATE / PHYSICS_SOLVER_ITERATIONS);
		m_AccumulatedForce = LLGP::Vector2f::zero;
	}
}
