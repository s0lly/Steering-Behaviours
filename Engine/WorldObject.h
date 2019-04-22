#pragma once

#include "Brain.h"
#include "PhysicsInfo.h"
#include "Vec2.h"

class WorldObject
{
public:

	WorldObject(PhysicsInfo inPhysicsInfo, float in_mass, float in_maxSpeed, Color in_color)
		:
		physicsInfo(inPhysicsInfo),
		mass(in_mass),
		maxSpeed(in_maxSpeed),
		color(in_color)
	{
		switch (inPhysicsInfo.brainType)
		{
		case BRAIN_TYPE::SEEKER:
		{
			brainPtr = new SeekerBrain(physicsInfo);
		}break;
		case BRAIN_TYPE::SCARED:
		{
			brainPtr = new ScaredBrain(physicsInfo);
		}break;
		}
	}

	void DetermineAction(std::vector<PhysicsInfo> &worldObjectPhysicsInfos)
	{
		brainPtr->Refresh(physicsInfo);

		desiredVelocity = brainPtr->DetermineDesiredVelocity(worldObjectPhysicsInfos);
	}

	//void CalculateNeighbourInfo(WorldObject &targetObject)
	//{
	//	Vec2 toTarget = targetObject.GetLoc() - GetLoc();
	//	float toTargetDist = toTarget.GetMagnitude();
	//
	//	float SEPERATION_DISTANCE = 400.0f;
	//	float ALIGNMENT_DISTANCE = 600.0f;
	//	float COHESION_DISTANCE = 2000.0f;
	//
	//	float angleSignToTarget = velocity.Dot(toTarget);
	//	float angleToTarget = acosf(velocity.Dot(toTarget));
	//	bool isSeen = (angleSignToTarget > 0.0f); // 180 degree FOV
	//
	//	//isSeen = true;
	//
	//	if ((toTargetDist < SEPERATION_DISTANCE) && isSeen)
	//	{
	//		// Seperation
	//		seperationVelocity = seperationVelocity - toTarget.Normalize() / toTargetDist;
	//		numSeperationTargets++;
	//	}
	//
	//	if (toTargetDist < ALIGNMENT_DISTANCE && isSeen)
	//	{
	//	
	//		// Alignment
	//		alignmentVelocity = alignmentVelocity + targetObject.GetVelocity().Normalize();
	//		numAlignmentTargets++;
	//	}
	//	
	//	
	//	if (toTargetDist < COHESION_DISTANCE && isSeen)
	//	{
	//		// Cohesion
	//		neighbourAverageLoc = neighbourAverageLoc + targetObject.GetLoc();
	//		numCohesionTargets++;
	//	}
	//}

	void Move(float dt)
	{
		physicsInfo.loc = physicsInfo.loc + physicsInfo.velocity * dt;
	}

	void Update(float dt)
	{
		//float SEPERATION_MULTIPLIER = (float)numSeperationTargets / 2.0f;
		//float COHESION_MULTIPLIER = (float)numCohesionTargets / 4.0f;
		//float ALIGNMENT_MULTIPLIER = (float)numAlignmentTargets * 8.0f;
		//float RANDOMNESS_MULTIPLIER = 1.0f;
		//
		//float MAX_FORCE = 1.0f;
		//
		//
		//// Cohesion
		//if (numCohesionTargets > 0)
		//{
		//	neighbourAverageLoc = neighbourAverageLoc / numCohesionTargets;
		//	cohesionVelocity = (neighbourAverageLoc - loc);
		//}
		//
		//
		//Vec2 randomnessVelocity = Vec2((float)(rand() % 101 - 50), (float)(rand() % 101 - 50));
		//
		//desiredVelocity = seperationVelocity.Normalize() * SEPERATION_MULTIPLIER;
		//
		//if (true)
		//{
		//	desiredVelocity = desiredVelocity + cohesionVelocity.Normalize() * COHESION_MULTIPLIER;
		//}
		//
		//if (true)
		//{
		//	desiredVelocity = desiredVelocity + alignmentVelocity.Normalize() * ALIGNMENT_MULTIPLIER;
		//}
		//
		//if (true)
		//{
		//	desiredVelocity = desiredVelocity + randomnessVelocity.Normalize() * RANDOMNESS_MULTIPLIER;;
		//}

		//if (desiredVelocity.GetMagnitude() < 0.001f)
		//{
		//	desiredVelocity = physicsInfo.velocity;
		//}
		//else
		//{
		//	
		//}

		desiredVelocity = (desiredVelocity).Normalize() * maxSpeed;

		ApplyForce(desiredVelocity - physicsInfo.velocity, dt);

		Move(dt);
	}

	void ApplyForce(Vec2 newForce, float dt)
	{
		physicsInfo.velocity = physicsInfo.velocity + (newForce * dt) / mass;
	}

	PhysicsInfo GetPhysicsInfo()
	{
		return physicsInfo;
	}

	float GetMass()
	{
		return mass;
	}

	float GetMaxSpeed()
	{
		return maxSpeed;
	}

	Color GetColor()
	{
		return color;
	}

	float GetRadius()
	{
		return radius;
	}

	//void ResetNeighbourInfo()
	//{
	//	seperationVelocity = Vec2();
	//	alignmentVelocity = Vec2();
	//	cohesionVelocity = Vec2();
	//	neighbourAverageLoc = Vec2();
	//	numSeperationTargets = 0;
	//	numCohesionTargets = 0;
	//	numAlignmentTargets = 0;
	//}


private:

	PhysicsInfo physicsInfo;
	float mass;
	float maxSpeed;
	float radius = 5.0f;
	Color color = Colors::Red;
	Vec2 desiredVelocity;
	Brain* brainPtr = nullptr;
	//Vec2 seperationVelocity;
	//Vec2 alignmentVelocity;
	//Vec2 cohesionVelocity;
	//Vec2 neighbourAverageLoc;
	//int numSeperationTargets;
	//int numAlignmentTargets;
	//int numCohesionTargets;
	
};