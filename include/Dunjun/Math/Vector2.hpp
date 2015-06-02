#ifndef DUNJUN_MATH_VECTOR2_HPP
#define DUNJUN_MATH_VECTOR2_HPP

#include <Dunjun/Types.hpp>
#include <iostream>
#include <cmath>

namespace Dunjun
{
struct Vector2
{
	Vector2()
	: x(0)
	, y(0)
	{
	}

	explicit Vector2(f32 xy)
	: x(xy)
	, y(xy)
	{
	}

	Vector2(f32 x, f32 y)
	: x(x)
	, y(y)
	{
	}

	Vector2(f32 xy[2])
	: x(xy[0])
	, y(xy[1])
	{
	}

	Vector2(const Vector2& other) = default;

	f32& operator[](usize index) { return data[index]; }
	const f32& operator[](usize index) const { return data[index]; }

	bool operator==(const Vector2& other) const
	{
		for (usize i = 0; i < 2; i++)
		{
			if (data[i] != other.data[i])
			{
				return false;
			}

			return true;
		}
	}

	bool operator!=(const Vector2& other) const { return !operator==(other); }

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator*(f32 scalar) const
	{
		return Vector2(scalar * x, scalar * y);
	}

	Vector2 operator/(f32 scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}

	// Hardmard Product
	Vector2 operator*(Vector2& other) const
	{
		Vector2 result;

		for (usize i = 0; i < 2; i++)
		{
			result[i] = data[i] * other.data[i];
		}

		return result;
	}

	Vector2& operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2& operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vector2& operator*=(f32 scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vector2& operator/=(f32 scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	union
	{
		f32 data[2];
		struct
		{
			f32 x, y; // Position / Data
		};
		struct
		{
			f32 r, g; // Color
		};
		struct
		{
			f32 s, t; // Texture position
		};
	};
};

inline Vector2 operator*(f32 scalar, const Vector2& vector)
{
	return vector * scalar;
}

inline f32 dot(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

inline f32 cross(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.x * vec2.y - vec2.x * vec1.y;
}

inline f32 lengthSquared(const Vector2& vec) { return dot(vec, vec); }

inline f32 length(const Vector2& vec) { return std::sqrtf(lengthSquared(vec)); }

inline Vector2 normalize(const Vector2& vec)
{
	return vec * (1.0f / length(vec));
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
	return os << "Vector2(" << vec[0] << ", " << vec[1] << ")";
}

} // namespace Dunjun
#endif