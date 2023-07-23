#pragma once
#include <math.h>
#include <Core/arithmetic_concpet.h>

namespace LLGP
{
	template<typename T> requires arithmetic<T>
	class Vector3
	{
	public:
		static const Vector3 zero;
		static const Vector3 one;
		static const Vector3 right;
		static const Vector3 left;
		static const Vector3 up;
		static const Vector3 down;
		static const Vector3 forward;
		static const Vector3 backward;

		T x, y, z;

#pragma region constructors
		Vector3() : x(0), y(0), z(0) {}
		Vector3(const Vector3&) = default;
		Vector3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

		template<typename U> requires arithmetic<U>
		explicit Vector3(const Vector3<U>& in) :
			x(static_cast<T>(in.x)), y(static_cast<T>(in.y), z(static_cast<T>(in.z)) {}

#pragma endregion
	};

	typedef Vector3<int>			Vector3i;
	typedef Vector3<unsigned int>	Vector3u;
	typedef Vector3<double>			Vector3d;
}
