#pragma once
#include <Core/Vector3.h>
#include <concepts>

namespace LLGP
{

#ifndef CONCEPT_ARITHMETIC
#define CONCEPT_ARITHMETIC
	//C++20 concept created so templates can require arithmetic properties of tempalted typenames
	template<typename T>
	concept arithmetic = std::integral<T> or std::floating_point<T>;
#endif

	template<typename T>
		requires arithmetic<T>
	class Vector2
	{
	public:
		static const Vector2 zero;
		static const Vector2 one;
		static const Vector2 right;
		static const Vector2 left;
		static const Vector2 up;
		static const Vector2 down;

		T x, y;

#pragma region constructors
		Vector2() : x(0), y(0) {}
		Vector2(const Vector2&) = default;
		Vector2(T X, T Y) : x(X), y(Y) {}

		template<typename U>
		explicit Vector2(const Vector2<U>& in) :
			x(static_cast<T>(in.x)), y(static_cast<T>(in.y)) {}

		template<typename U>
		Vector2(const Vector3<U>& in) :
			x(static_cast<T>(in.x)), y(static_cast<T>(in.y)) {}
#pragma endregion

		Vector2<T> operator-(const Vector2<T>& rhs) { return Vector2<T>(-rhs.x, -rhs.y); }
		Vector2<T>& operator+=(const Vector2<T>& a) { x += a.x; y += a.y; return *this; }
		friend Vector2<T> operator+(Vector2<T> lhs, const Vector2<T>& rhs) { lhs += rhs; return lhs; }
		Vector2<T>& operator-=(const Vector2<T>& a) { x -= a.x; y -= a.y; return *this; }
		friend Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs) { lhs -= rhs; return lhs; }

		template<typename U>
			requires arithmetic<U>
		Vector2<T>& operator*=(const U a) { x *= a; y *= a; return *this; }

		template<typename U>
			requires arithmetic<U>
		friend Vector2<T> operator*(Vector2<T> v, const U a) { v *= a; return v; }
		template<typename U>
			requires arithmetic<U>
		friend Vector2<T> operator*(const U a, Vector2<T> v) { v *= a; return v; }
		
		template<typename U>
			requires arithmetic<U>
		Vector2<T>& operator/=(const U a) { x /= a; y /= a; return *this; }
		template<typename U>
			requires arithmetic<U>
		friend Vector2<T> operator/(Vector2<T> v, const U a) { v /= a; return v; }
	};

	// Define the most common types
	typedef Vector2<int>			Vector2i;
	typedef Vector2<unsigned int>	Vector2u;
	typedef Vector2<double>			Vector2d;
}
