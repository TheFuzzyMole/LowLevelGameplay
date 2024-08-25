#pragma once

#include <Core/Object.h>
#include <Core/ISerializable.h>

namespace LLGP
{

	class GameObject;
	struct Collision;

	class Component : public Object, public ISerializable
	{
	public:
		Component(GameObject* owner);
		Component(GameObject* owner, YAML::Node inData);
		~Component();

		GameObject* GetGameObject() { return _GameObject; }
		
		virtual void OwnerActiveChange(bool newActive) {}

		virtual void OnCollisionEnter(Collision* collision) {}
		virtual void OnCollisionStay(Collision* collision) {}
		virtual void OnCollisionExit(Collision* collision) {}

		virtual void Start() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void Destroy() {}

		// Inherited via ISerializable
		void Serialize(YAML::Emitter& out) override = 0;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override = 0;
	
	protected:
		GameObject* _GameObject;
	};
}


