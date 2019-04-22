#pragma once

#include <math.h>

#define MAGNITUDE_THRESHOLD 0.00001f

struct Vec2
{
	Vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	Vec2(float a, float b)
		:
		x(a),
		y(b)
	{
	}

	Vec2 operator +(Vec2& rhs)
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator -(Vec2& rhs)
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator *(float rhs)
	{
		return Vec2(x * rhs, y * rhs);
	}

	Vec2 operator /(float rhs)
	{
		return Vec2(x / rhs, y / rhs);
	}

	float GetMagnitudeSqrd()
	{
		return x * x + y * y;
	}

	float GetMagnitude()
	{
		return sqrt(GetMagnitudeSqrd());
	}

	Vec2 Normalize()
	{
		float magnitude = GetMagnitude();
		if (magnitude > MAGNITUDE_THRESHOLD)
		{
			return Vec2(x / magnitude, y / magnitude);
		}
		else
		{
			return Vec2(0.0f, 0.0f);
		}
	}

	float Dot(Vec2& rhs)
	{
		Vec2 norm = Normalize();
		Vec2 otherNorm = rhs.Normalize();

		return norm.x * otherNorm.x + norm.y * otherNorm.y;
	}

	float x;
	float y;
};