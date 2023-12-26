#pragma once

#include "Object.h"

namespace LLGP::Core { class GameObject; }

namespace LLGP::Core
{
	class Component : public Object
	{
	public:
		Component(GameObject* owner) : _GameObject(owner) {}
		virtual ~Component() {}

		GameObject* GetGameObject() { return _GameObject; }
		
		virtual void OwnerActiveChange(bool newActive) {}
	protected:
		GameObject* _GameObject;
	};
}
