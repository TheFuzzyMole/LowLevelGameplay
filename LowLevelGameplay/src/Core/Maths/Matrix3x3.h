#pragma once
#include <concepts>
#include <Core/Maths/Vector2.h>
#include <Core/Maths/Maths.h>

namespace LLGP
{
	template<typename T> requires std::is_arithmetic_v<T>
	struct Matrix3x3
	{
		Matrix3x3() = default;
		Matrix3x3(T _x0, T _y0, T _z0, T _x1, T _y1, T _z1, T _x2, T _y2, T _z2)
			: x0(_x0), y0(_y0), z0(_z0), x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2) {}
		Matrix3x3(const Matrix3x3&) = default;

		T x0, y0, z0, x1, y1, z1, x2, y2, z2;

		Matrix3x3<T>& Inverse();
		Matrix3x3<T> Inversed() const;
	
		template<typename U> requires std::is_arithmetic_v<U>
		static Matrix3x3<T> FromPos(const LLGP::Vector2<U>& pos);
		template<typename V> requires std::is_arithmetic_v<V>
		static Matrix3x3<T> FromRot(const V& rot);
		template<typename W> requires std::is_arithmetic_v<W>
		static Matrix3x3<T> FromScale(const LLGP::Vector2<W>& scale);

		template<typename U, typename V> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<V>
		static Matrix3x3<T> FromPR(const LLGP::Vector2<U>& pos, const V& rot);
		template<typename U, typename W> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<W>
		static Matrix3x3<T> FromPS(const LLGP::Vector2<U>& pos, const LLGP::Vector2<W>& scale);

		template<typename V, typename W> requires std::is_arithmetic_v<V> && std::is_arithmetic_v<W>
		static Matrix3x3<T> FromRS(const V& rot, const LLGP::Vector2<W>& scale);

		template<typename U, typename V, typename W> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<V> && std::is_arithmetic_v<W>
		static Matrix3x3<T> FromPRS(const LLGP::Vector2<U>& pos, const V& rot, const LLGP::Vector2<W> scale);

		template<typename U, typename V, typename W> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<V> && std::is_arithmetic_v<W>
		static void Decompose(LLGP::Matrix3x3<T> m, LLGP::Vector2<U>& pos, V& rot, LLGP::Vector2<W>& scale);

		static const Matrix3x3 identity;
	};

	//seemingly the wrong way round so that the right side modifies
	
	/// <summary>
	/// The input will modify the matrix being set and "happen second" in the chain for usability
	/// </summary>
	template<typename T> requires std::is_arithmetic_v<T>
	Matrix3x3<T>& operator*=(Matrix3x3<T>& a, const Matrix3x3<T>& b)
	{
		Matrix3x3<T> temp;
		temp.x0 = b.x0 * a.x0 + b.y0 * a.x1 + b.z0 * a.x2;
		temp.y0 = b.x0 * a.y0 + b.y0 * a.y1 + b.z0 * a.y2;
		temp.z0 = b.x0 * a.z0 + b.y0 * a.z1 + b.z0 * a.z2;

		temp.x1 = b.x1 * a.x0 + b.y1 * a.x1 + b.z1 * a.x2;
		temp.y1 = b.x1 * a.y0 + b.y1 * a.y1 + b.z1 * a.y2;
		temp.z1 = b.x1 * a.z0 + b.y1 * a.z1 + b.z1 * a.z2;

		temp.x2 = b.x2 * a.x0 + b.y2 * a.x1 + b.z2 * a.x2;
		temp.y2 = b.x2 * a.y0 + b.y2 * a.y1 + b.z2 * a.y2;
		temp.z2 = b.x2 * a.z0 + b.y2 * a.z1 + b.z2 * a.z2;

		return a = temp;
	}

	//b is the base and a modifies
	template<typename T> requires std::is_arithmetic_v<T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& a, const Matrix3x3<T>& b)
	{
		Matrix3x3<T> temp;
		temp.x0 = a.x0 * b.x0 + a.y0 * b.x1 + a.z0 * b.x2;
		temp.y0 = a.x0 * b.y0 + a.y0 * b.y1 + a.z0 * b.y2;
		temp.z0 = a.x0 * b.z0 + a.y0 * b.z1 + a.z0 * b.z2;

		temp.x1 = a.x1 * b.x0 + a.y1 * b.x1 + a.z1 * b.x2;
		temp.y1 = a.x1 * b.y0 + a.y1 * b.y1 + a.z1 * b.y2;
		temp.z1 = a.x1 * b.z0 + a.y1 * b.z1 + a.z1 * b.z2;

		temp.x2 = a.x2 * b.x0 + a.y2 * b.x1 + a.z2 * b.x2;
		temp.y2 = a.x2 * b.y0 + a.y2 * b.y1 + a.z2 * b.y2;
		temp.z2 = a.x2 * b.z0 + a.y2 * b.z1 + a.z2 * b.z2;

		return temp;
	}

	template<typename T, typename U> requires std::is_arithmetic_v<T> and std::is_arithmetic_v<U>
	Matrix3x3<T>& operator*=(Matrix3x3<T>& m, const U a)
	{
		T Ta = static_cast<T>(a);
		m.x0 = (m.x0 * Ta);
		m.y0 = (m.y0 * Ta);
		m.z0 = (m.z0 * Ta);
		m.x1 = (m.x1 * Ta);
		m.y1 = (m.y1 * Ta);
		m.z1 = (m.z1 * Ta);
		m.x2 = (m.x2 * Ta);
		m.y2 = (m.y2 * Ta);
		m.z2 = (m.z2 * Ta);
		//m.x0 = static_cast<T>(static_cast<U>(m.x0) * a);
		//m.y0 = static_cast<T>(static_cast<U>(m.y0) * a);
		//m.z0 = static_cast<T>(static_cast<U>(m.z0) * a);
		//m.x1 = static_cast<T>(static_cast<U>(m.x1) * a);
		//m.y1 = static_cast<T>(static_cast<U>(m.y1) * a);
		//m.z1 = static_cast<T>(static_cast<U>(m.z1) * a);
		//m.x2 = static_cast<T>(static_cast<U>(m.x2) * a);
		//m.y2 = static_cast<T>(static_cast<U>(m.y2) * a);
		//m.z2 = static_cast<T>(static_cast<U>(m.z2) * a);
		return m;
	}

	template<typename T, typename U> requires std::is_arithmetic_v<T> and std::is_arithmetic_v<U>
	Matrix3x3<T> operator*(Matrix3x3<T> m, const U a)
	{
		return m *= a;
	}

	template<typename T, typename U> requires std::is_arithmetic_v<T> and std::is_arithmetic_v<U>
	LLGP::Vector2<U> operator*(const LLGP::Vector2<U>& v, const Matrix3x3<T>& m)
	{
		/*
		* in -> x   y   1     out
		*       x0  y0  z0 -> x*x0 + y*y0 + z0
		*       x1  y1  z1 -> x*x1 + y*y1 + z1
		*       0   0   1  -> 1
		*/
		return LLGP::Vector2<U>(v.x * m.x0 + v.y * m.y0 + m.z0,
								v.x * m.x1 + v.y * m.y1 + m.z1);
	}

	template<typename T, typename U> requires std::is_arithmetic_v<T> and std::is_arithmetic_v<U>
	LLGP::Vector2<U>& operator*=(LLGP::Vector2<U>& v, const LLGP::Matrix3x3<T>& m)
	{
		v = LLGP::Vector2<U>(v.x * m.x0 + v.y * m.y0 + m.z0,
			v.x * m.x1 + v.y * m.y1 + m.z1);
		return v;
	}

	template<typename T> requires std::is_arithmetic_v<T>
	Matrix3x3<T>& Matrix3x3<T>::Inverse()
	{
		/*
		*   1     | d -b |
		* ----- x |      |
		* ad-bc   |-c  a |
		* 
		* a=x0
		* b=y0
		* c=x1
		* d=y1
		*/
		T a = x0, b = y0, c = x1, d = y1;
		T det = (a * d - b * c);
		x0 = d / det; y0 = -b / det;
		x1 = -c / det; y1 = a / det;

		T x = z0, y = z1;
		z0 = -(x * x0 + y * y0);
		z1 = -(x * x1 + y * y1);
		return *this;
	}

	template<typename T> requires std::is_arithmetic_v<T>
	Matrix3x3<T> Matrix3x3<T>::Inversed() const
	{
		Matrix3x3<T> toReturn = *this;
		return toReturn.Inverse();
	}

#pragma region (De)Composition

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename U> requires std::is_arithmetic_v<U>
	inline Matrix3x3<T> Matrix3x3<T>::FromPos(const LLGP::Vector2<U>& pos)
	{
		return Matrix3x3<T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(pos.x),
							static_cast<T>(0), static_cast<T>(1), static_cast<T>(pos.y),
							static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
	}

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename V> requires std::is_arithmetic_v<V>
	inline Matrix3x3<T> Matrix3x3<T>::FromRot(const V& rot)
	{
		V _rot = static_cast<V>(dmod((double)rot + (2.0 * PI), 2.0 * PI));
		return Matrix3x3<T>(static_cast<T>(cos(_rot)), static_cast<T>(-sin(_rot)), static_cast<T>(0),
							static_cast<T>(sin(_rot)), static_cast<T>(cos(_rot)), static_cast<T>(0),
							static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
	}

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename W> requires std::is_arithmetic_v<W>
	inline Matrix3x3<T> Matrix3x3<T>::FromScale(const LLGP::Vector2<W>& scale)
	{
		return Matrix3x3<T>(static_cast<T>(scale.x), static_cast<T>(0), static_cast<T>(0),
							static_cast<T>(0), static_cast<T>(scale.y), static_cast<T>(0),
							static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
	}

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename U, typename V> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<V>
	inline Matrix3x3<T> Matrix3x3<T>::FromPR(const LLGP::Vector2<U>& pos, const V& rot)
	{
		return (Matrix3x3<T>::FromPos(pos) * Matrix3x3<T>::FromRot(rot));
	}

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename U, typename W> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<W>
	inline Matrix3x3<T> Matrix3x3<T>::FromPS(const LLGP::Vector2<U>& pos, const LLGP::Vector2<W>& scale)
	{
		return (Matrix3x3<T>::FromPos(pos) * Matrix3x3<T>::FromScale(scale));
	}

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename V, typename W> requires std::is_arithmetic_v<V> && std::is_arithmetic_v<W>
	inline Matrix3x3<T> Matrix3x3<T>::FromRS(const V& rot, const LLGP::Vector2<W>& scale)
	{
		return (Matrix3x3<T>::FromRot(rot) * Matrix3x3<T>::FromScale(scale));
	}

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename U, typename V, typename W> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<V> && std::is_arithmetic_v<W>
	inline Matrix3x3<T> Matrix3x3<T>::FromPRS(const LLGP::Vector2<U>& pos, const V& rot, const LLGP::Vector2<W> scale)
	{
		return Matrix3x3<T>::FromPos(pos) * (Matrix3x3<T>::FromRot(rot) * Matrix3x3<T>::FromScale(scale));
	}

	template<typename T> requires std::is_arithmetic_v<T>
	template<typename U, typename V, typename W> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<V> && std::is_arithmetic_v<W>
	inline void Matrix3x3<T>::Decompose(LLGP::Matrix3x3<T> m, LLGP::Vector2<U>& pos, V& rot, LLGP::Vector2<W>& scale)
	{
		pos = LLGP::Vector2<U>( static_cast<U>(m.z0), static_cast<U>(m.z1));
		m.z0 = static_cast<T>(0);
		m.z1 = static_cast<T>(0);

		scale = LLGP::Vector2<W>(	static_cast<W>(LLGP::Vector2<T>(m.x0, m.x1).GetMagnitude()),
									static_cast<W>(LLGP::Vector2<T>(m.y0, m.y1).GetMagnitude()));

		m.x0 /= scale.x; m.y0 /= scale.y;
		m.x1 /= scale.x; m.y1 /= scale.y;

		rot = static_cast<V>(asin(m.x1));
	}
	
#pragma endregion //(De)Composition

	template<typename T> requires std::is_arithmetic_v<T>
	const Matrix3x3<T> Matrix3x3<T>::identity(	static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
												static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
												static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));

	typedef Matrix3x3<float>		Mat3f;
	typedef Matrix3x3<double>		Mat3d;
	
}
