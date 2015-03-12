#ifndef DUNJUN_MATH_VECTOR3_HPP
#define DUNJUN_MATH_VECTOR3_HPP

#include <Dunjun/Types.hpp>
#include <cmath>

#include <Dunjun/Math/Vector2.hpp>

namespace Dunjun
{
struct Vector3
{
	Vector3()
	: x(0)
	, y(0)
	, z(0)
	{
	}

	explicit Vector3(f32 xyz)
	: x(xyz)
	, y(xyz)
	, z(xyz)
	{
	}

	Vector3(f32 x, f32 y, f32 z)
	: x(x)
	, y(y)
	, z(z)
	{
	}

	Vector3(f32 xyz[3])
	: x(xyz[0])
	, y(xyz[1])
	, z(xyz[2])
	{
	}

	explicit Vector3(const Vector2& vec, f32 z)
	: x(vec.x)
	, y(vec.y)
	, z(z)
	{
	}

	f32& operator[](usize index) { return data[index]; }
	const f32& operator[](usize index) const { return data[index]; }

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator*(f32 scalar) const
	{
		return Vector3(scalar * x, scalar * y, scalar * z);
	}

	Vector3 operator/(f32 scalar) const
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	Vector3& operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector3& operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z += other.z;

		return *this;
	}

	Vector3& operator*=(f32 scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vector3& operator/=(f32 scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	union
	{
		f32 data[3];
		struct
		{
			f32 x, y, z; // Position / Data
		};
		struct
		{
			f32 r, g, b; // Color
		};
		struct
		{
			f32 s, t, p; // Texture position
		};
	};
};

inline Vector3 operator*(f32 scalar, const Vector3& vector)
{
	return vector * scalar;
}

inline f32 dot(const Vector3& vec1, const Vector3& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline Vector3 cross(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(vec1.y * vec2.z - vec2.y * vec1.z, // x
	               vec1.z * vec2.x - vec2.z * vec1.x, // y
	               vec1.x * vec2.y - vec2.x * vec1.y  // z
	               );
}

inline f32 lengthSquared(const Vector3& vec) { return dot(vec, vec); }

inline f32 length(const Vector3& vec) { return lengthSquared(vec); }

inline Vector3 normalized(const Vector3& vec)
{
	return vec * (1.0f / length(vec));
}

} // namespace Dunjun

#endif