#pragma once
#include <concepts>
#include <Core/Vector2.h>
#include <Utils/Maths.h>

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
	template<typename U, typename V, typename W> requires std::is_arithmetic_v<U> && std::is_arithmetic_v<V>&& std::is_arithmetic_v<W>
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
