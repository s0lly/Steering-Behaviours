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

class Line
{

public:

	Line() {}

	Line(Vec2 v0, Vec2 v1)
	{
		// if vertical line
		if (!(v0.x == v1.x))
		{
			m = (v1.y - v0.y) / (v1.x - v0.x);
			c = v1.y - m * v1.x;

			if (m > 1.0f || m < -1.0f)
			{
				reversedLine = true;
			}
		}
		else
		{
			reversedLine = true;
		}

		if (reversedLine)
		{
			m = (v1.x - v0.x) / (v1.y - v0.y);
			c = v1.x - m * v1.y;
		}
	}

	float GetY(float x)
	{
		if (!reversedLine)
		{
			return m * x + c;
		}
		else
		{
			return (x - c) / m;
		}
	}

	float GetX(float y)
	{
		if (!reversedLine)
		{
			return (y - c) / m;
		}
		else
		{
			return m * y + c;
		}
	}

	float m = 0.0f;
	float c = 0.0f;
	bool reversedLine = false;

private:

};