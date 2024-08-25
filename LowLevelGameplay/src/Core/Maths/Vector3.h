#pragma once
#include <math.h>
#include <Core/arithmetic_concept.h>
#include <Core/Vector2.h>

namespace LLGP
{
	template<typename T> requires arithmetic<T>
	struct Vector2;

	template<typename T> requires arithmetic<T>
	struct Vector3
	{
	public:

		T x, y, z;

#pragma region constructors
		Vector3() : x(0), y(0), z(0) {}
		Vector3(const Vector3&) = default;
		Vector3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

		template<typename U> requires arithmetic<U>
		explicit Vector3(const Vector3<U>& in) :
			x(static_cast<T>(in.x)), y(static_cast<T>(in.y)), z(static_cast<T>(in.z)) {}

		template<typename U> requires arithmetic<U>
		operator Vector2<U>() { return Vector2<U>(static_cast<U>(x), static_cast<U>(y)); }
#pragma endregion

		float GetSqrMagnitude() { return x * x + y * y + z * z; }
		float GetMagnitude() { return sqrt(GetSqrMagnitude()); }
		Vector3<T>& Normalise() { *this / GetMagnitude(); return *this; }
		Vector3<T> Normalised() { return *this / GetMagnitude(); }

		static float Dot(const Vector3<T>& lhs, const Vector3<T>& rhs) { return (float)(lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z); }
		static float Angle(const Vector3<T>& lhs, const Vector3<T>& rhs) { return acos(Dot(lhs.Normalised(), rhs.Normalised())); }

		static Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs) { return Vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.x * rhs.z - rhs.z * lhs.x, lhs.x * rhs.y - lhs.y * rhs.x); }

		static const Vector3 zero;
		static const Vector3 one;
		static const Vector3 right;
		static const Vector3 left;
		static const Vector3 up;
		static const Vector3 down;
		static const Vector3 forward;
		static const Vector3 backward;
	};

	template<typename T> requires arithmetic<T>
	Vector3<T> operator-(const Vector3<T>& rhs) { return Vector3<T>(-rhs.x, -rhs.y, -rhs.z); }

	template<typename T> requires arithmetic<T>
	Vector3<T>& operator+=(Vector3<T>& lhs, const Vector3<T>& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; return lhs; }

	template<typename T> requires arithmetic<T>
	Vector3<T> operator+(Vector3<T> lhs, const Vector3<T>& rhs) { return lhs += rhs; }

	template<typename T> requires arithmetic<T>
	Vector3<T>& operator-=(Vector3<T>& lhs, const Vector3<T>& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; return lhs; }

	template<typename T> requires arithmetic<T>
	Vector3<T> operator-(Vector3<T> lhs, const Vector3<T>& rhs) { return lhs -= rhs; }

	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector3<T>& operator*=(Vector3<T>& v, const U& a) { v.x *= a; v.y *= a; v.z *= a; return v; }

	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector3<T> operator*(Vector3<T> v, const U& a) { return v *= a; }

	template<typename T, typename U> requires arithmetic<T> and arithmetic <U>
	Vector3<T> operator*(const U& a, Vector3<T> v) { return v *= a; }
	
	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector3<T>& operator/=(Vector3<T>& v, const U& a) { v.x /= a; v.y /= a; v.z /= a; return v; }

	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector3<T> operator/(Vector3<T> v, const U& a) { return v /= a; }
	
	template<typename T> requires arithmetic<T>
	bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs) { Vector3<T> dist = lhs - rhs; float num4 = dist.x * dist.x + dist.y * dist.y + dist.z * dist.z; return num4 < 9.99999944E-11F; }

	template<typename T> requires arithmetic<T>
	bool operator!=(const Vector3<T>& lhs, const Vector3<T>& rhs) { return !(lhs == rhs); }
	
	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::zero(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));

	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::one(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1));

	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::right(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));

	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::left(-static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));

	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::up(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));

	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::down(static_cast<T>(0), -static_cast<T>(1), static_cast<T>(0));

	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::forward(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));

	template<typename T> requires arithmetic<T>
	const Vector3<T> Vector3<T>::backward(static_cast<T>(0), static_cast<T>(0), -static_cast<T>(1));

	typedef Vector3<int>			Vector3i;
	typedef Vector3<unsigned int>	Vector3u;
	typedef Vector3<double>			Vector3d;
}
