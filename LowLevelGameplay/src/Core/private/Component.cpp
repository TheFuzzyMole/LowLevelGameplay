#include <Core/Component.h>
#include <Core/GameObject.h>

namespace LLGP
{
	Component::Component(GameObject* owner) : _GameObject(owner)
	{
		//owner->OnCollisionEnter += std::bind(&Component::OnCollisionEnter, this, std::placeholders::_1);
		//owner->OnCollisionStay += std::bind(&Component::OnCollisionStay, this, std::placeholders::_1);
		//owner->OnCollisionExit += std::bind(&Component::OnCollisionExit, this, std::placeholders::_1);
	}

	Component::~Component()
	{
		//_GameObject->OnCollisionEnter -= std::bind(&Component::OnCollisionEnter, this, std::placeholders::_1);
		//_GameObject->OnCollisionStay -= std::bind(&Component::OnCollisionStay, this, std::placeholders::_1);
		//_GameObject->OnCollisionExit -= std::bind(&Component::OnCollisionExit, this, std::placeholders::_1);
	}
}
