#pragma once

#include "Brain.h"
#include "PhysicsInfo.h"
#include "Vec2.h"

class WorldObject
{
public:

	WorldObject(PhysicsInfo *in_PhysicsInfoPtr, float in_mass, Color in_color)
		:
		physicsInfoPtr(in_PhysicsInfoPtr),
		mass(in_mass),
		color(in_color)
	{
		switch (physicsInfoPtr->brainType)
		{
		case BRAIN_TYPE::SHARK:
		{
			brainPtr = new SharkBrain(physicsInfoPtr);
		}break;
		case BRAIN_TYPE::FISH:
		{
			brainPtr = new FishBrain(physicsInfoPtr);
		}break;
		case BRAIN_TYPE::BEE:
		{
			brainPtr = new BeeBrain(physicsInfoPtr);
		}break;
		}
	}

	void DetermineAction(std::vector<PhysicsInfo*> worldObjectPhysicsInfos)
	{
		brainPtr->Refresh();

		brainPtr->DetermineVelocities(worldObjectPhysicsInfos);
	}


	void Move(float dt)
	{
		physicsInfoPtr->loc = physicsInfoPtr->loc + physicsInfoPtr->velocity * dt;
	}

	void Update(float dt)
	{
		// Cap desiredVelocity at maxSpeed
		//if (physicsInfo.totalDesiredVelocity.GetMagnitude() > physicsInfo.maxSpeed)
		//{
		//	physicsInfo.desiredVelocity = (physicsInfo.desiredVelocity).Normalize() * physicsInfo.maxSpeed;
		//}

		ApplyForce(physicsInfoPtr->totalSteeringVelocity, dt);

		Move(dt);
	}

	void ApplyForce(Vec2 newForce, float dt)
	{
		physicsInfoPtr->velocity = physicsInfoPtr->velocity + (newForce * dt) / mass;
	}

	PhysicsInfo * GetPhysicsInfo()
	{
		return physicsInfoPtr;
	}

	float GetMass()
	{
		return mass;
	}

	float GetMaxSpeed()
	{
		return physicsInfoPtr->maxSpeed;
	}

	Color GetColor()
	{
		return color;
	}

	float GetRadius()
	{
		return radius;
	}


//private:

	PhysicsInfo *physicsInfoPtr;
	float mass;
	float radius = 5.0f;
	Color color = Colors::Red;
	Brain* brainPtr = nullptr;

	
};




//Vec2 seperationVelocity;
//Vec2 alignmentVelocity;
//Vec2 cohesionVelocity;
//Vec2 neighbourAverageLoc;
//int numSeperationTargets;
//int numAlignmentTargets;
//int numCohesionTargets;


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
