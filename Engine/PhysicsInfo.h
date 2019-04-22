#pragma once

#include "Vec2.h"

enum BRAIN_TYPE
{
	SEEKER,
	SCARED,
	PURSUER,
	EVADER,
	WANDERER
};

enum BEHAVIOUR_TYPE
{
	SEEK,
	FLEE,
	ALIGN,
	//ARRIVE,
	//PURSUE,
	//EVADE,
	//WANDER,
	NUM_BEHAVIOURS
};

struct PhysicsInfo
{
	PhysicsInfo()
	{
	}

	PhysicsInfo(const PhysicsInfo& in_physicsInfo)
		:
		brainType(in_physicsInfo.brainType),
		loc(in_physicsInfo.loc),
		velocity(in_physicsInfo.velocity),
		ID(in_physicsInfo.ID),
		relativeDistSqrd(in_physicsInfo.relativeDistSqrd),
		relativeLoc(in_physicsInfo.relativeLoc)
	{
	}

	PhysicsInfo(BRAIN_TYPE in_brainType, Vec2 in_loc, Vec2 in_velocity, int in_ID)
		:
		brainType(in_brainType),
		loc(in_loc),
		velocity(in_velocity),
		ID(in_ID)
	{
	}

	bool operator<(PhysicsInfo& other)
	{
		return (relativeDistSqrd < other.relativeDistSqrd);
	}

	void SetRelativeInfo(Vec2 locToCompare, Vec2 viewerVelocity)
	{
		if (!(loc.x == locToCompare.x && loc.y == locToCompare.y))
		{
			float angle = 0.0f;

			if (viewerVelocity.GetMagnitude() < 0.01f)
			{
				angle = (float)(rand() % 6282) / 1000.0f - 3.141f;
			}
			else
			{
				angle = acosf(viewerVelocity.y / viewerVelocity.GetMagnitude());
				//if (velocity.y < 0.0f)
				//{
				//	angle = 3.14159f - angle;
				//}
				if (viewerVelocity.x < 0.0f)
				{
					angle = -angle;
				}
			}
			
			float cosAngle = cos(angle);
			float sinAngle = sin(angle);
			relativeDistSqrd = (loc - locToCompare).GetMagnitudeSqrd();
			relativeLoc.x = (loc - locToCompare).x * cosAngle - (loc - locToCompare).y * sinAngle;
			relativeLoc.y = (loc - locToCompare).y * cosAngle + (loc - locToCompare).x * sinAngle;

			isInEyesight = ((relativeLoc.y > 0) && (abs(relativeLoc.x) < relativeLoc.y));
		}
		else
		{
			relativeDistSqrd = 0.0f;
			relativeLoc = Vec2();
		}
	}

	BRAIN_TYPE brainType;
	Vec2 loc;
	Vec2 velocity;
	int ID;
	float relativeDistSqrd;
	Vec2 relativeLoc;
	bool isInEyesight;
};