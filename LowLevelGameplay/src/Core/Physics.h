#pragma once
#include <vector>
#include <memory>
#include <Core/Vector2.h>
#include <Core/Event.h>

namespace LLGP
{
	class Collider;
	class CircleCollider;
	class BoxCollider;
	class Rigidbody;

	struct Collision
	{
	public:
		Collider* collider;
		Collider* otherCollider;
		Vector2<float> point;
		Vector2<float> normal;
		Vector2<float> impulse;

		bool operator==(Collision* b); 
		bool operator!=(Collision* b);
	};

	class Physics
	{
	public:
		Physics() = delete;

		static LLGP::Vector2f Gravity;

		static LLGP::Event<> OnStepPhysics;
		
		static bool RegisterCollider(Collider* c);
		static void RemoveCollider(Collider* c);

		static void StepPhysics();
		static void CollectCollisions();
		static void DispatchCollisions();

		static Collision* Collision_CircleCircle(CircleCollider* a, CircleCollider* b);
		static Collision* Collision_AABBAABB(BoxCollider* a, BoxCollider* b);
		static Collision* Collision_AABBCircle(BoxCollider* a, CircleCollider* b);
		static Collision* Collision_AABBCircle(CircleCollider* a, BoxCollider* b);

	private:
		static std::vector<Collider*> _Colliders;
		static std::vector<Rigidbody*> _Rigidbodies;
		static std::vector<Collision*> _Collisions;
		static std::vector<Collision*> _OldCollisions;
	};
}
