#pragma once
#include <Core/Component.h>
#include <Core/Vector2.h>

namespace LLGP
{
	enum ForceMode
	{
		Force,
		Acceleration,
		Impulse,
		VelocityChange
	};

	class Rigidbody : public Component
	{
	public:
		Rigidbody(GameObject* owner);
		Rigidbody(GameObject* owner, YAML::Node inData);
		~Rigidbody();

		LLGP::Vector2f Velocity;
		float Mass;
		bool IsKinematic;
		bool HasGravity;

		void AddForce(LLGP::Vector2f force, LLGP::ForceMode mode = LLGP::ForceMode::Force);

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node) override;
	private:
		LLGP::Vector2f m_AccumulatedForce;

		void ApplyAccumulatedForce();
	};
}
