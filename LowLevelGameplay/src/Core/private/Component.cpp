#include <Core/Component.h>
#include <Core/GameObject.h>

namespace LLGP
{
	Component::Component(GameObject* owner) : _GameObject(owner)
	{
		owner->OnCollisionEnter.AddListener(this, std::bind(&Component::OnCollisionEnter, this, std::placeholders::_1));
		owner->OnCollisionStay.AddListener(this, std::bind(&Component::OnCollisionStay, this, std::placeholders::_1));
		owner->OnCollisionExit.AddListener(this, std::bind(&Component::OnCollisionExit, this, std::placeholders::_1));
	}

	Component::~Component()
	{
		_GameObject->OnCollisionEnter.RemoveListener(this, std::bind(&Component::OnCollisionEnter, this, std::placeholders::_1));
		_GameObject->OnCollisionStay.RemoveListener(this, std::bind(&Component::OnCollisionStay, this, std::placeholders::_1));
		_GameObject->OnCollisionExit.RemoveListener(this, std::bind(&Component::OnCollisionExit, this, std::placeholders::_1));
	}
}
