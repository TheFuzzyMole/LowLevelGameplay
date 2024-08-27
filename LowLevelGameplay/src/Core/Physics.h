#pragma once
#include <vector>
#include <memory>
#include <Core/Maths/Vector2.h>
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
		bool otherHasRB = false;
		Vector2<float> point;
		Vector2<float> normal;
		float overlap;
		float impulseMagnitude;

		bool operator==(Collision* b);
	};

	class Physics
	{
	public:
		Physics() = delete;

		static inline LLGP::Vector2f Gravity;

		static inline LLGP::Event<> OnStepPhysics;
		
		static bool RegisterCollider(LLGP::Collider* c);
		static void RemoveCollider(LLGP::Collider* c);
		
		static bool RegisterRigidbody(LLGP::Rigidbody* c);
		static void RemoveRigidbody(LLGP::Rigidbody* c);

		static void StepPhysics();
		static void CollectCollisions();
		static void ResolveCollisionOverlaps();
		static void DispatchCollisions();
		static void DispatchOverlaps();

		static Collision* Collision_CircleCircle(CircleCollider* a, CircleCollider* b);
		static Collision* Collision_AABBAABB(BoxCollider* a, BoxCollider* b);
		static Collision* Collision_AABBCircle(BoxCollider* a, CircleCollider* b);
		static Collision* Collision_AABBCircle(CircleCollider* a, BoxCollider* b);

	private:
		static Collision* ReverseCollision(Collision* in);

		static inline std::vector<Collider*> _Colliders;
		static inline std::vector<Collider*> _Triggers;
		static inline std::vector<Rigidbody*> _Rigidbodies;
		static inline std::vector<Collision*> _IterationCollisions;
		static inline std::vector<Collision*> _Collisions;
		static inline std::vector<Collision*> _Overlaps;
		static inline std::vector<Collision*> _OldCollisions;
		static inline std::vector<Collision*> _OldOverlaps;
		static inline std::vector<Collision*> _ReversedCollisions;
	};
}
