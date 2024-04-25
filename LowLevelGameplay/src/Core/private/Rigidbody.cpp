#include <Core/Rigidbody.h>
#include <Core/Physics.h>
#include <Core/Commons.h>
#include <Core/GameObject.h>

namespace LLGP
{
	Rigidbody::Rigidbody(GameObject* owner)
		: Component(owner)
	{
		Velocity = LLGP::Vector2f::zero;
		Mass = 0.f;
		m_AccumulatedForce = LLGP::Vector2f::zero;

		//FIX: Physics::OnStepPhysics += std::bind(&Rigidbody::ApplyAccumulatedForce, this);
	}

	Rigidbody::~Rigidbody()
	{
		//FIX: Physics::OnStepPhysics -= std::bind(&Rigidbody::ApplyAccumulatedForce, this);
	}

	void Rigidbody::AddForce(LLGP::Vector2f force, LLGP::ForceMode mode)
	{
		switch (mode)
		{
		case LLGP::Force:
			m_AccumulatedForce += (force / Mass) * FIXED_FRAMERATE;
			break;
		case LLGP::Acceleration:
			m_AccumulatedForce += (force / Mass);
			break;
		case LLGP::Impulse:
			m_AccumulatedForce += force * FIXED_FRAMERATE;
			break;
		case LLGP::VelocityChange:
			m_AccumulatedForce += force;
			break;
		}
	}

	void Rigidbody::ApplyAccumulatedForce()
	{
		Velocity += m_AccumulatedForce;
		_GameObject->transform->ChangePosition(Velocity * FIXED_FRAMERATE);
	}
}
