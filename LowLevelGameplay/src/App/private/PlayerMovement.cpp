#include <App/PlayerMovement.h>
#include <Core/GameObject.h>
#include <Core/Time.h>
#include <Core/Components/Rigidbody.h>
#include <Core/Commons.h>
#include <cassert>

namespace TEST
{
	PlayerMovement::PlayerMovement(LLGP::GameObject* owner) :
		LLGP::Component(owner), m_Speed(0.f), m_InMoveVector(LLGP::Vector2f::zero), m_IsMoving(false)
	{
		m_RB = _GameObject->GetComponent<LLGP::Rigidbody>();
		LLGP::InputManager::FindAction("Move")->OnHeld.AddListener(this, std::bind(&PlayerMovement::Handle_MovePerformed, this, std::placeholders::_1));
		LLGP::InputManager::FindAction("Move")->OnCancelled.AddListener(this, std::bind(&PlayerMovement::Handle_MoveCancelled, this, std::placeholders::_1));
	}

	void PlayerMovement::Start()
	{
		if (m_RB == nullptr) { m_RB = _GameObject->GetComponent<LLGP::Rigidbody>(); }
		assert(m_RB != nullptr);
	}

	void PlayerMovement::SetSpeed(float _speed)
	{
		m_Speed = _speed;
	}

	void PlayerMovement::Handle_MovePerformed(std::variant<bool, float, LLGP::Vector2f> context)
	{
		m_InMoveVector = std::get<2>(context);
		if (!m_IsMoving)
		{
			//start the coroutine thread
			_GameObject->OnWorldFixedUpdate.AddListener(this, std::bind(&PlayerMovement::C_MoveUpdate, this));
			//m_CRMoveUpdate = std::jthread(std::bind_front(&PlayerMovement::C_MoveUpdate, this));
			m_IsMoving = true;
		}
	}

	void PlayerMovement::Handle_MoveCancelled(std::variant<bool, float, LLGP::Vector2f> context)
	{
		m_InMoveVector = std::get<2>(context);
		if (m_IsMoving)
		{
			_GameObject->OnWorldFixedUpdate.RemoveListener(this, std::bind(&PlayerMovement::C_MoveUpdate, this));
			m_IsMoving = false;
		}
	}
	void PlayerMovement::C_MoveUpdate()
	{
		m_RB->AddForce(m_InMoveVector * m_Speed, LLGP::ForceMode::Acceleration);
		//std::shared_lock lock(m_CRm);
		//NextFixedUpdate.wait(lock);
	}
    void PlayerMovement::Serialize(YAML::Emitter& out)
    {
		out << YAML::Key << "PlayerMovement" << YAML::Value << YAML::BeginMap; // PlayerMovement
		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::Key << "Speed" << YAML::Value << m_Speed;

		out << YAML::EndMap; //PlayerMovement
    }
    bool PlayerMovement::Deserialize(YAML::Node node, std::vector<LLGP::LinkRequest>& linkRequests)
    {
		if (!node["Speed"]) { return false; }
		m_Speed = node["Speed"].as<float>();
		return true;
    }
}
