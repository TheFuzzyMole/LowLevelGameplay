#pragma once

#include <Core/Object.h>

namespace LLGP
{
	class GameObject;
	struct Collision;

	class Component : public Object
	{
	public:
		Component(GameObject* owner);
		~Component();

		GameObject* GetGameObject() { return _GameObject; }
		
		virtual void OwnerActiveChange(bool newActive) {}

		virtual void OnCollisionEnter(Collision* collision) {}
		virtual void OnCollisionStay(Collision* collision) {}
		virtual void OnCollisionExit(Collision* collision) {}

		virtual void Start() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}

	protected:
		GameObject* _GameObject;

	};
}


