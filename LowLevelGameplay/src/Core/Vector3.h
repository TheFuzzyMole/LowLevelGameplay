#pragma once
#include <Core/Vector2.h>

namespace LLGP
{
	template<typename T>
	class Vector3
	{
	public:
		T x, y, z;

		Vector3() = default;
		Vector3(const Vector3&) = default;
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		//Vector3(const Vector2& _in) : x(_in.x), y(_in.y), z(0.f) {}

	};
}
