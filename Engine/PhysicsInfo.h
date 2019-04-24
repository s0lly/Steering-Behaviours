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
	COHESION,
	SEPARATION,
	//ARRIVE,
	//PURSUE,
	//EVADE,
	//WANDER,
	NUM_BEHAVIOURS
};

struct BehaviourInfo
{
	std::string name;
	BEHAVIOUR_TYPE type;
	BRAIN_TYPE targetBrainType;
	float addTargetsInDistSqrdMin; // keep??
	float addTargetsInDistSqrdMax;
	float intensity;
	int maxTargets;
	int currentTargets;
	Vec2 desiredVelocity;
	Vec2 steeringVelocity;
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
		relativeLoc(in_physicsInfo.relativeLoc),
		isInEyesight(in_physicsInfo.isInEyesight),
		totalDesiredVelocity(in_physicsInfo.totalDesiredVelocity),
		totalSteeringVelocity(in_physicsInfo.totalSteeringVelocity),
		maxSpeed(in_physicsInfo.maxSpeed)
	{
	}

	PhysicsInfo(BRAIN_TYPE in_brainType, Vec2 in_loc, Vec2 in_velocity, int in_ID, float in_maxSpeed)
		:
		brainType(in_brainType),
		loc(in_loc),
		velocity(in_velocity),
		ID(in_ID),
		maxSpeed(in_maxSpeed)
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

			if (viewerVelocity.GetMagnitude() < 1.0f)
			{
				angle = (float)(rand() % 6282) / 1000.0f - 3.141f;
				//angle = 0.0f;
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

			//isInEyesight = ((relativeLoc.y > 0)); // 180degree FOV 
			isInEyesight = true; // 360degree FOV
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
	Vec2 totalDesiredVelocity;
	Vec2 totalSteeringVelocity;
	float maxSpeed;
};