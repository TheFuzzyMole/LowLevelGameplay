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

	bool Collision::operator==(Collision* b) { return (*(this->collider) == *(b->collider)) && (*(this->otherCollider) == *(b->otherCollider)); }
	bool Collision::operator!=(Collision* b) { return !(this == b); }

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

	void Physics::StepPhysics()
	{
		Physics::OnStepPhysics();
	}

	void Physics::CollectCollisions()
	{
		if (_Colliders.size() < 2) { return; }

		//TODO: change this to be a check from colliders on Rigidbodies agains all other colliders if they arent the same
		for (int rb = 0; rb < _Rigidbodies.size(); rb++)
		{
			std::vector<Collider*> rbCols = _Rigidbodies[rb]->GetGameObject()->GetComponents<Collider>();
			for (int colIndex = 0; colIndex < rbCols.size(); colIndex++)
			{
				for (int worldCol = 0; worldCol < _Colliders.size(); worldCol++)
				{
					if (rbCols[colIndex] == _Colliders[worldCol] || *rbCols[colIndex]->GetGameObject() == *_Colliders[worldCol]->GetGameObject()) { continue; }
					_Collisions.push_back(rbCols[colIndex]->IsColliding(_Colliders[worldCol]));
				}
			}
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

					delete _OldCollisions[oldCol];
					_OldCollisions.erase(std::next(_OldCollisions.begin(), oldCol));
					break;
				}
			}
			if (newCollision)
			{
				_Collisions[newCol]->collider->GetGameObject()->OnCollisionEnter(_Collisions[newCol]);
			}
		}

		//deal with remaining old collisions
		for (Collision* c : _OldCollisions)
		{
			c->collider->GetGameObject()->OnCollisionExit(c);
			delete c;
		}
		_OldCollisions.clear();
		_OldCollisions = _Collisions;
		_Collisions.clear();
	}

	LLGP::Collision* Physics::Collision_CircleCircle(CircleCollider* a, CircleCollider* b)
	{
		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition(); //Done as A->B
		float radiusSum = a->GetRadius() + b->GetRadius();
		if (radiusSum * radiusSum > diff.GetSqrMagnitude()) return nullptr;

		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->normal = diff.Normalised();
		toReturn->point = a->GetPosition() + (toReturn->normal * a->GetRadius());
		toReturn->impulse = LLGP::Vector2f::zero;
		return toReturn;
	}
	LLGP::Collision* Physics::Collision_AABBAABB(BoxCollider* a, BoxCollider* b)
	{
		//a.left < b.right && a.right > b.left && a.bottom < b.top && a.top > b.bottom
		LLGP::Vector2<float> aMax, aMin, bMax, bMin;
		aMax = a->GetPosition() + a->GetExtents();
		aMin = a->GetPosition() - a->GetExtents();
		bMax = b->GetPosition() + b->GetExtents();
		bMin = b->GetPosition() - b->GetExtents();
		if (aMin.x >= bMax.x || aMax.x <= bMin.x || aMin.y >= bMax.y || aMax.y <= bMin.y) return nullptr;

		LLGP::Vector2f diff = b->GetPosition() - a->GetPosition();
		LLGP::Vector2f dir = diff.Normalised();

		LLGP::Collision* toReturn = new LLGP::Collision();
		toReturn->collider = a;
		toReturn->otherCollider = b;
		toReturn->normal = LLGP::Vector2f(copysignf(floor(abs(dir.x) + 0.5f), diff.x), copysignf(floor(abs(dir.y) + 0.5f), diff.y));
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
		toReturn->normal = diff.Normalised();
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
		toReturn->normal = -toReturn->normal;
		toReturn->impulse = -toReturn->impulse;
		return toReturn;
	}
}
