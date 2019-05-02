#pragma once

#include "Vec2.h"

enum BRAIN_TYPE
{
	SHARK,
	FISH,
	BEE,
	NONE
};

enum BEHAVIOUR_TYPE
{
	SEEK,
	FLEE,
	ARRIVE,
	PURSUE,
	EVADE,
	ALIGN,
	WANDER,
	SEPARATION,
	SLOW_DOWN,
	//ARRIVE,
	//PURSUE,
	//EVADE,
	//WANDER,
	NUM_BEHAVIOURS
};

enum EYESIGHT_RANGE
{
	FOV_90,
	FOV_180,
	FOV_270,
	FOV_360
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
	Vec2 steeringVelocity;
	bool isActive = true;
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
		totalDesiredVelocity(in_physicsInfo.totalDesiredVelocity),
		totalSteeringVelocity(in_physicsInfo.totalSteeringVelocity),
		maxSpeed(in_physicsInfo.maxSpeed),
		fov(in_physicsInfo.fov)
	{
	}

	PhysicsInfo(BRAIN_TYPE in_brainType, Vec2 in_loc, Vec2 in_velocity, int in_ID, float in_maxSpeed, EYESIGHT_RANGE in_fov)
		:
		brainType(in_brainType),
		loc(in_loc),
		velocity(in_velocity),
		ID(in_ID),
		maxSpeed(in_maxSpeed),
		fov(in_fov)
	{
	}

	BRAIN_TYPE brainType;
	Vec2 loc;
	Vec2 velocity;
	int ID;
	Vec2 totalDesiredVelocity;
	Vec2 totalSteeringVelocity;
	float maxSpeed;
	EYESIGHT_RANGE fov;
};


struct RelativeInfo
{
	float relativeDistSqrd;
	Vec2 relativeLoc;
	bool isInEyesight;
	int ID;

	bool operator<(RelativeInfo& other)
	{
		return (relativeDistSqrd < other.relativeDistSqrd);
	}

	void SetRelativeInfo(PhysicsInfo* viewer, PhysicsInfo* target)
	{
		ID = target->ID;
		isInEyesight = false;
		relativeDistSqrd = 0.0f;
		relativeLoc = Vec2();

		if (viewer->ID != target->ID)
		{
			float angle = 0.0f;

			if (viewer->velocity.GetMagnitude() < 1.0f)
			{
				angle = (float)(rand() % 6282) / 1000.0f - 3.141f;
				//angle = 0.0f;
			}
			else
			{
				angle = acosf(viewer->velocity.y / viewer->velocity.GetMagnitude());
				//if (velocity.y < 0.0f)
				//{
				//	angle = 3.14159f - angle;
				//}
				if (viewer->velocity.x < 0.0f)
				{
					angle = -angle;
				}
			}

			float cosAngle = cos(angle);
			float sinAngle = sin(angle);
			relativeDistSqrd = (target->loc - viewer->loc).GetMagnitudeSqrd();
			relativeLoc.x = (target->loc - viewer->loc).x * cosAngle - (target->loc - viewer->loc).y * sinAngle;
			relativeLoc.y = (target->loc - viewer->loc).y * cosAngle + (target->loc - viewer->loc).x * sinAngle;

			switch (viewer->fov)
			{
				case EYESIGHT_RANGE::FOV_90:
				{
					isInEyesight = ((relativeLoc.y > 0) && (abs(relativeLoc.x) < relativeLoc.y)); // 90degree FOV 
				}break;

				case EYESIGHT_RANGE::FOV_180:
				{
					isInEyesight = ((relativeLoc.y > 0)); // 180degree FOV 
				}break;

				case EYESIGHT_RANGE::FOV_270:
				{
					isInEyesight = ((relativeLoc.y > 0) || ((relativeLoc.y <= 0) && ((abs(relativeLoc.x) > abs(relativeLoc.y))))); // 270degree FOV 
				}break;

				case EYESIGHT_RANGE::FOV_360:
				{
					isInEyesight = true; // 360degree FOV
				}break;

			}
			
		}
	}
};