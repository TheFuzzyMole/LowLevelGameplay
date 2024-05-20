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
			Rigidbody* aRB = collision->collider->GetGameObject()->GetComponent<Rigidbody>();
			Rigidbody* bRB = collision->otherCollider->GetGameObject()->GetComponent<Rigidbody>();
			float aMass = aRB ? aRB->Mass : 0.f;
			float bMass = bRB ? bRB->Mass : 0.f;
			if (!bRB) { bMass = aMass; aMass = 0.f; }

			collision->collider->GetGameObject()->transform->ChangePosition(collision->normal * (collision->overlap * (-bMass / (aMass + bMass))));
			collision->otherCollider->GetGameObject()->transform->ChangePosition(collision->normal * (collision->overlap * (aMass / (aMass + bMass))));
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

	LLGP::Collision* Physics::Collision_CircleCircle(CircleCollider* a, CircleCollider* b)
	{
		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition(); //Done as A->B
		float radiusSum = a->GetRadius() + b->GetRadius();
		if (radiusSum * radiusSum > diff.GetSqrMagnitude()) return nullptr;

		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->otherHasRB = toReturn->otherCollider->GetGameObject()->GetComponent<Rigidbody>();
		toReturn->normal = diff.Normalised();
		toReturn->overlap = radiusSum - diff.GetMagnitude();
		toReturn->point = a->GetPosition() + (toReturn->normal * a->GetRadius());
		toReturn->impulse = LLGP::Vector2f::zero;
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

		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition();
		LLGP::Vector2f dir = diff.Normalised();
		LLGP::Vector2f exts = aExt + bExt;
		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->otherHasRB = toReturn->otherCollider->GetGameObject()->GetComponent<Rigidbody>();
		float xMult = abs(diff.x) / exts.x; float yMult = abs(diff.y) / exts.y;
		toReturn->normal = LLGP::Vector2f(copysign((xMult >= yMult ? 1.f : 0.f), diff.x), copysignf(floor(yMult >= xMult ? 1.f : 0.f), diff.y));
		toReturn->overlap = LLGP::Vector2f((aExt.x + bExt.x - abs(diff.x)) * toReturn->normal.x, (aExt.y + bExt.y - abs(diff.y)) * toReturn->normal.y).GetMagnitude();
		toReturn->point = LLGP::Vector2f(fmaxf(dir.x, toReturn->normal.x), fmaxf(dir.y, toReturn->normal.y));
		toReturn->impulse = LLGP::Vector2f::zero;
		return toReturn;
	}
	LLGP::Collision* Physics::Collision_AABBCircle(BoxCollider* a, CircleCollider* b)
	{
		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition();
		LLGP::Vector2f dir = LLGP::Vector2f(copysignf(1.f, diff.x), copysignf(1.f, diff.y));
		LLGP::Vector2f ext = a->GetExtents();
		float rad = b->GetRadius();

		if((abs(diff.x) >= (ext.x + rad) || abs(diff.y) >= (ext.y + rad)) ||
			((diff - LLGP::Vector2f(dir.x * ext.x, dir.y * ext.y)).GetSqrMagnitude() >= rad * rad)) return nullptr;
		
		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->otherHasRB = toReturn->otherCollider->GetGameObject()->GetComponent<Rigidbody>();
		toReturn->normal = diff.Normalised();
		toReturn->overlap = LLGP::Vector2f(ext.x + rad - diff.x,ext.y + rad - diff.y).GetMagnitude();
		toReturn->point = b->GetPosition() - (toReturn->normal * rad);
		toReturn->impulse = LLGP::Vector2f::zero;
		return toReturn;
	}
	LLGP::Collision* Physics::Collision_AABBCircle(CircleCollider* a, BoxCollider* b)
	{
		LLGP::Collision* toReturn = Collision_AABBCircle(b, a);
		Collider* temp = toReturn->collider;
		toReturn->collider = toReturn->otherCollider;
		toReturn->otherCollider = temp;
		toReturn->otherHasRB = toReturn->otherCollider->GetGameObject()->GetComponent<Rigidbody>();
		toReturn->normal = -toReturn->normal;
		toReturn->impulse = -toReturn->impulse;
		return toReturn;
	}

	Collision* Physics::ReverseCollision(Collision* in)
	{
		Collision* reverse = new Collision();
		reverse->collider = in->otherCollider;
		reverse->otherCollider = in->collider;

		reverse->normal = -in->normal;
		reverse->impulse = -in->impulse;
		_ReversedCollisions.push_back(reverse);
		return reverse;
	}
}
