#include <Core/Physics.h>
#include <Core/CircleCollider.h>
#include <Core/BoxCollider.h>
#include <Core/Rigidbody.h>
#include <Core/GameObject.h>
#include <cmath>

namespace LLGP
{
	LLGP::Vector2f Physics::Gravity = LLGP::Vector2f(0.f, 9.814f);
	LLGP::Event<> Physics::OnStepPhysics;

	std::vector<Collider*> Physics::_Colliders;
	std::vector<Rigidbody*> Physics::_Rigidbodies;
	std::vector<Collision*> Physics::_Collisions;
	std::vector<Collision*> Physics::_OldCollisions;
	std::vector<Collision*> Physics::_ReversedCollisions;

	bool Collision::operator==(Collision* b) { return ((*(this->collider) == *(b->collider)) && (*(this->otherCollider) == *(b->otherCollider))
													|| (*(this->collider) == *(b->otherCollider) && (*(this->otherCollider) == *(b->collider)))); }

	bool Physics::RegisterCollider(Collider* c)
	{
		if (std::find_if(_Colliders.begin(), _Colliders.end(), [c](Collider* check) {return *check == *c; }) == _Colliders.end())
		{
			_Colliders.push_back(c);
			return true;
		}
		return false;
	}

	void Physics::RemoveCollider(Collider* c)
	{
		std::erase_if(_Colliders, [c](Collider* check) {return *check == *c; });
	}

	bool Physics::RegisterRigidbody(LLGP::Rigidbody* c)
	{
		if (std::find_if(_Rigidbodies.begin(), _Rigidbodies.end(), [c](LLGP::Rigidbody* check) {return *check == *c; }) == _Rigidbodies.end())
		{
			_Rigidbodies.push_back(c);
			return true;
		}
		return false;
	}

	void Physics::RemoveRigidbody(LLGP::Rigidbody* c)
	{
		std::erase_if(_Rigidbodies, [c](LLGP::Rigidbody* check) {return *check == *c; });
	}

	void Physics::StepPhysics()
	{
		Physics::OnStepPhysics();
	}

	void Physics::CollectCollisions()
	{
		if (_Colliders.size() < 2) { return; }

		_Collisions.clear();

		//TODO: change this to be a check from colliders on Rigidbodies agains all other colliders if they arent the same
		for (int rb = 0; rb < _Rigidbodies.size(); rb++)
		{
			std::vector<Collider*> rbCols = _Rigidbodies[rb]->GetGameObject()->GetComponents<Collider>();
			for (int colIndex = 0; colIndex < rbCols.size(); colIndex++)
			{
				for (int worldCol = 0; worldCol < _Colliders.size(); worldCol++)
				{
					if (rbCols[colIndex] == _Colliders[worldCol] || *rbCols[colIndex]->GetGameObject() == *_Colliders[worldCol]->GetGameObject()) { continue; }

					{
						Collision testCol; testCol.collider = rbCols[colIndex]; testCol.otherCollider = _Colliders[worldCol];
						if (std::find_if(_Collisions.begin(), _Collisions.end(), [&testCol](Collision* other) { return testCol == other; }) != _Collisions.end()) { continue; }
					}

					if (Collision* collision = rbCols[colIndex]->IsColliding(_Colliders[worldCol]))
					{
						_Collisions.push_back(collision);
					}
				}
			}
		}
	}

	void Physics::ResolveOverlaps()
	{
		for (Collision* collision : _Collisions)
		{
			LLGP::Rigidbody* aRB = collision->collider->GetGameObject()->GetComponent<Rigidbody>();
			LLGP::Rigidbody* bRB = collision->otherCollider->GetGameObject()->GetComponent<Rigidbody>();
			float aMass = aRB ? aRB->Mass : 0.f;
			float bMass = bRB ? bRB->Mass : 0.f;
			if (!bRB) { bMass = aMass; aMass = 0.f; }

			collision->collider->GetGameObject()->transform->ChangePosition(collision->normal * (collision->overlap * (-bMass / (aMass + bMass))));
			collision->otherCollider->GetGameObject()->transform->ChangePosition(collision->normal * (collision->overlap * (aMass / (aMass + bMass))));

			aRB->AddForce(collision->normal * -abs(collision->impulseMagnitude), LLGP::ForceMode::Impulse);
			if (bRB) { bRB->AddForce(collision->normal * abs(collision->impulseMagnitude), LLGP::ForceMode::Impulse); }
		}
	}

	void Physics::DispatchCollisions()
	{
		for (int newCol = (int)_Collisions.size() - 1; newCol >= 0; newCol--)
		{
			bool newCollision = true;
			for (int oldCol = (int)_OldCollisions.size() - 1; oldCol >= 0; oldCol--)
			{
				if (_Collisions[newCol] == _OldCollisions[oldCol])
				{
					newCollision = false;
					_Collisions[newCol]->collider->GetGameObject()->OnCollisionStay(_Collisions[newCol]);
					if (_Collisions[newCol]->otherHasRB) { _Collisions[newCol]->otherCollider->GetGameObject()->OnCollisionStay(ReverseCollision(_Collisions[newCol])); }

					delete _OldCollisions[oldCol];
					_OldCollisions.erase(std::next(_OldCollisions.begin(), oldCol));
					break;
				}
			}
			if (newCollision)
			{
				_Collisions[newCol]->collider->GetGameObject()->OnCollisionEnter(_Collisions[newCol]);
				if (_Collisions[newCol]->otherHasRB) { _Collisions[newCol]->otherCollider->GetGameObject()->OnCollisionEnter(ReverseCollision(_Collisions[newCol])); }
			}
		}

		//deal with remaining old collisions
		for (int oldIndex = _OldCollisions.size() - 1; oldIndex >= 0; oldIndex--)
		{
			_OldCollisions[oldIndex]->collider->GetGameObject()->OnCollisionExit(_OldCollisions[oldIndex]);
			if (_OldCollisions[oldIndex]->otherHasRB) { _OldCollisions[oldIndex]->otherCollider->GetGameObject()->OnCollisionExit(ReverseCollision(_OldCollisions[oldIndex])); }
			delete _OldCollisions[oldIndex];
		}

		_OldCollisions.clear();
		_OldCollisions = _Collisions;
		_Collisions.clear();
		
		for (Collision* c : _ReversedCollisions) { delete c; }
		_ReversedCollisions.clear();
	}

#pragma region collision maths
	LLGP::Collision* Physics::Collision_CircleCircle(CircleCollider* a, CircleCollider* b)
	{
		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition(); //Done as A->B
		float radiusSum = a->GetRadius() + b->GetRadius();
		if (radiusSum * radiusSum > diff.GetSqrMagnitude()) return nullptr;

		LLGP::Rigidbody* aRB = a->GetGameObject()->GetComponent<Rigidbody>();
		LLGP::Rigidbody* bRB = b->GetGameObject()->GetComponent<Rigidbody>();


		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->otherHasRB = bRB != nullptr;
		toReturn->normal = diff.Normalised();
		toReturn->overlap = radiusSum - diff.GetMagnitude();
		toReturn->point = a->GetPosition() + (toReturn->normal * a->GetRadius());
		float restitution = fminf(a->GetRestitution(), b->GetRestitution());
		toReturn->impulseMagnitude = (-(1 + restitution) * LLGP::Vector2f::Dot((bRB ? bRB->Velocity : Vector2f::zero) - aRB->Velocity, toReturn->normal)) / ((1 / aRB->Mass) + (bRB ? 1 / bRB->Mass : 0));
		//toReturn->impulseMagnitude = LLGP::Vector2f::Dot(aRB->Velocity, toReturn->normal) * aRB->Mass + (bRB != nullptr ? LLGP::Vector2f::Dot(bRB->Velocity, -toReturn->normal) * bRB->Mass : 0.f);
		return toReturn;
	}
	LLGP::Collision* Physics::Collision_AABBAABB(BoxCollider* a, BoxCollider* b)
	{
		//a.left < b.right && a.right > b.left && a.bottom < b.top && a.top > b.bottom
		LLGP::Vector2f aMax, aMin, bMax, bMin;
		LLGP::Vector2f aExt = a->GetExtents();
		LLGP::Vector2f bExt = b->GetExtents();
		aMax = a->GetPosition() + aExt;
		aMin = a->GetPosition() - aExt;
		bMax = b->GetPosition() + bExt;
		bMin = b->GetPosition() - bExt;
		if (aMin.x >= bMax.x || aMax.x <= bMin.x || aMin.y >= bMax.y || aMax.y <= bMin.y) return nullptr;

		LLGP::Rigidbody* aRB = a->GetGameObject()->GetComponent<Rigidbody>();
		LLGP::Rigidbody* bRB = b->GetGameObject()->GetComponent<Rigidbody>();

		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition();
		LLGP::Vector2f dir = diff.Normalised();
		LLGP::Vector2f exts = aExt + bExt;
		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->otherHasRB = bRB != nullptr;
		float xMult = abs(diff.x) / exts.x; float yMult = abs(diff.y) / exts.y;
		toReturn->normal = LLGP::Vector2f(copysign((xMult >= yMult ? 1.f : 0.f), diff.x), copysignf(floor(yMult >= xMult ? 1.f : 0.f), diff.y));
		toReturn->overlap = LLGP::Vector2f((aExt.x + bExt.x - abs(diff.x)) * toReturn->normal.x, (aExt.y + bExt.y - abs(diff.y)) * toReturn->normal.y).GetMagnitude();
		toReturn->point = LLGP::Vector2f(fmaxf(dir.x, toReturn->normal.x), fmaxf(dir.y, toReturn->normal.y));
		float restitution = fminf(a->GetRestitution(), b->GetRestitution());
		toReturn->impulseMagnitude = (-(1 + restitution) * LLGP::Vector2f::Dot((bRB ? bRB->Velocity : Vector2f::zero) - aRB->Velocity, toReturn->normal)) / ((1 / aRB->Mass) + (bRB ? 1 / bRB->Mass : 0));
		//toReturn->impulseMagnitude = LLGP::Vector2f::Dot(aRB->Velocity, toReturn->normal) + (bRB != nullptr ? LLGP::Vector2f::Dot(bRB->Velocity, -toReturn->normal) : 0.f);
		return toReturn;
	}
	LLGP::Collision* Physics::Collision_AABBCircle(BoxCollider* a, CircleCollider* b)
	{
		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition();
		LLGP::Vector2f ext = a->GetExtents();
		float rad = b->GetRadius();
		LLGP::Vector2f clamped = LLGP::Vector2f(std::clamp(diff.x, -ext.x, ext.x), std::clamp(diff.y, -ext.y, ext.y));
		diff -= clamped;

		if(diff.GetSqrMagnitude() > rad * rad) return nullptr;
		
		LLGP::Rigidbody* aRB = a->GetGameObject()->GetComponent<Rigidbody>();
		LLGP::Rigidbody* bRB = b->GetGameObject()->GetComponent<Rigidbody>();

		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->otherHasRB = bRB != nullptr;

		toReturn->normal = diff.Normalised();
		toReturn->overlap = rad - diff.GetMagnitude();
		toReturn->point = b->GetPosition() - diff;
		float restitution = fminf(a->GetRestitution(), b->GetRestitution());
		toReturn->impulseMagnitude = (-(1 + restitution) * LLGP::Vector2f::Dot((bRB ? bRB->Velocity : Vector2f::zero) - aRB->Velocity, toReturn->normal)) / ((1/aRB->Mass) + (bRB ? 1/bRB->Mass : 0));
		//toReturn->impulseMagnitude = LLGP::Vector2f::Dot(aRB->Velocity, toReturn->normal) + (bRB != nullptr ? LLGP::Vector2f::Dot(bRB->Velocity, -toReturn->normal) : 0.f);
		return toReturn;
	}
	LLGP::Collision* Physics::Collision_AABBCircle(CircleCollider* a, BoxCollider* b)
	{
		LLGP::Collision* toReturn = Collision_AABBCircle(b, a);
		if (!toReturn) return nullptr;
		Collider* temp = toReturn->collider;
		toReturn->collider = toReturn->otherCollider;
		toReturn->otherCollider = temp;
		toReturn->otherHasRB = toReturn->otherCollider->GetGameObject()->HasComponent<Rigidbody>();
		toReturn->normal = -toReturn->normal;
		return toReturn;
	}
#pragma endregion

	Collision* Physics::ReverseCollision(Collision* in)
	{
		Collision* reverse = new Collision();
		reverse->collider = in->otherCollider;
		reverse->otherCollider = in->collider;
		reverse->normal = -in->normal;
		_ReversedCollisions.push_back(reverse);
		return reverse;
	}
}
