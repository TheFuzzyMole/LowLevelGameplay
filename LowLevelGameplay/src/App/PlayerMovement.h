#pragma once
#include <Core/Component.h>
#include <Core/InputManager.h>
#include <thread>
#include <shared_mutex>

namespace LLGP
{
	class GameObject;
	class Rigidbody;
}

namespace TEST
{
	class PlayerMovement : public LLGP::Component
	{
	public:
		PlayerMovement(LLGP::GameObject* owner);

		void SetSpeed(float _speed);

		// Inherited via Component
		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& node) override;

	private:
		float m_Speed;
		LLGP::Rigidbody* m_RB;
		LLGP::Vector2f m_InMoveVector;
		std::jthread m_CRMoveUpdate;
		std::shared_mutex m_CRm;
		bool m_IsMoving;

		void Handle_MovePerformed(std::variant<bool, float, LLGP::Vector2f> context);
		void Handle_MoveCancelled(std::variant<bool, float, LLGP::Vector2f> context);
		void C_MoveUpdate(std::stop_token token);

	};
}
