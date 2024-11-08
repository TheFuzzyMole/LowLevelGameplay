#include <Core/Components/Component.h>
#include <Core/GameObject.h>

namespace LLGP
{
	Component::Component(GameObject* owner) : _GameObject(owner)
	{
		owner->OnCollisionEnter.AddListener(this, std::bind(&Component::OnCollisionEnter, this, std::placeholders::_1));
		owner->OnCollisionStay.AddListener(this, std::bind(&Component::OnCollisionStay, this, std::placeholders::_1));
		owner->OnCollisionExit.AddListener(this, std::bind(&Component::OnCollisionExit, this, std::placeholders::_1));
		owner->OnStart.AddListener(this, std::bind(&Component::Start, this));
		owner->OnUpdate.AddListener(this, std::bind(&Component::Update, this));
		owner->OnFixedUpdate.AddListener(this, std::bind(&Component::FixedUpdate, this));
		owner->OnDestroy.AddListener(this, std::bind(&Component::Destroy, this));
	}

	Component::Component(GameObject* owner, YAML::Node inData) : Object(inData["UUID"].as<uint64_t>()), _GameObject(owner)
	{
		owner->OnCollisionEnter.AddListener(this, std::bind(&Component::OnCollisionEnter, this, std::placeholders::_1));
		owner->OnCollisionStay.AddListener(this, std::bind(&Component::OnCollisionStay, this, std::placeholders::_1));
		owner->OnCollisionExit.AddListener(this, std::bind(&Component::OnCollisionExit, this, std::placeholders::_1));
		owner->OnStart.AddListener(this, std::bind(&Component::Start, this));
		owner->OnUpdate.AddListener(this, std::bind(&Component::Update, this));
		owner->OnFixedUpdate.AddListener(this, std::bind(&Component::FixedUpdate, this));
		owner->OnDestroy.AddListener(this, std::bind(&Component::Destroy, this));
	}

	Component::~Component()
	{
		_GameObject->OnCollisionEnter.RemoveListener(this, std::bind(&Component::OnCollisionEnter, this, std::placeholders::_1));
		_GameObject->OnCollisionStay.RemoveListener(this, std::bind(&Component::OnCollisionStay, this, std::placeholders::_1));
		_GameObject->OnCollisionExit.RemoveListener(this, std::bind(&Component::OnCollisionExit, this, std::placeholders::_1));
		_GameObject->OnStart.RemoveListener(this, std::bind(&Component::Start, this));
		_GameObject->OnUpdate.RemoveListener(this, std::bind(&Component::Update, this));
		_GameObject->OnFixedUpdate.RemoveListener(this, std::bind(&Component::FixedUpdate, this));
		_GameObject->OnDestroy.RemoveListener(this, std::bind(&Component::Destroy, this));
	}
	
}
