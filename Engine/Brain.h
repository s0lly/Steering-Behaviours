#pragma once

#include "Behaviour.h"
#include "PhysicsInfo.h"
#include "Vec2.h"
#include <vector>
#include <algorithm>


class Brain
{
public:
	Brain(PhysicsInfo in_self)
		:
		self(in_self)
	{
		decisionMade = false;
		randomness = (float)(rand() % 100);
	}

	void Refresh(PhysicsInfo in_self)
	{
		self = in_self;

		for (int i = 0; i < behaviours.size(); i++)
		{
			delete behaviours[i];
		}
		behaviours.clear();

		decisionMade = false;

		FormBehaviours();
	}

	virtual Vec2 DetermineDesiredVelocity(std::vector<PhysicsInfo> worldObjectPhysicsInfos) = 0;

	void FormBehaviours()
	{
		for (int i = 0; i < BEHAVIOUR_TYPE::NUM_BEHAVIOURS; i++)
		{
			behaviours.push_back(CreateBehaviour(self, (BEHAVIOUR_TYPE)i));
		}
	}
	

protected:
	PhysicsInfo self;
	std::vector<Behaviour*> behaviours;
	bool decisionMade;
	float randomness;
};

// Seek to closest target
class SeekerBrain : public Brain
{
public:
	SeekerBrain(PhysicsInfo in_self)
		:
		Brain(in_self)
	{
		decisionMade = false;
	}

	Vec2 DetermineDesiredVelocity(std::vector<PhysicsInfo> worldObjectPhysicsInfos) override
	{
		for (int j = 0; j < worldObjectPhysicsInfos.size(); j++)
		{
			worldObjectPhysicsInfos[j].SetRelativeInfo(self.loc, self.velocity);
		}

		std::sort(worldObjectPhysicsInfos.begin(), worldObjectPhysicsInfos.end());

		int numSeekTargets = 0;
		int maxSeekTargets = 1;


		Vec2 desiredVelocity;

		for (int j = 0; j < worldObjectPhysicsInfos.size() && !decisionMade; j++)
		{
			if (self.ID != worldObjectPhysicsInfos[j].ID && worldObjectPhysicsInfos[j].relativeLoc.y > 0.0f)
			{
				if (worldObjectPhysicsInfos[j].relativeDistSqrd < SEEK_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SCARED && numSeekTargets < maxSeekTargets)
				{
					behaviours[BEHAVIOUR_TYPE::SEEK]->AddTarget(worldObjectPhysicsInfos[j]);
					numSeekTargets++;
					// Seek
					

					// Arrive
					//if (toTargetDist > 0)
					//{
					//	float speed = std::min(toTargetDist / 2.0f, maxSpeed);
					//	desiredVelocity = toTarget * speed / toTargetDist;
					//}

					//decisionMade = true;
				}
				
				if (worldObjectPhysicsInfos[j].relativeDistSqrd < AVOID_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SEEKER)
				{
					// Flee
					behaviours[BEHAVIOUR_TYPE::FLEE]->AddTarget(worldObjectPhysicsInfos[j]);
				
					//decisionMade = true;
				}
			}
		}

		Vec2 fleeVec = behaviours[BEHAVIOUR_TYPE::FLEE]->PerformLogicAndGetVelocityVector();
		Vec2 seekVec = behaviours[BEHAVIOUR_TYPE::SEEK]->PerformLogicAndGetVelocityVector();

		if ((fleeVec).GetMagnitude() > 0.00001f)
		{
			desiredVelocity = desiredVelocity + fleeVec.Normalize() / (fleeVec).GetMagnitude();
		}

		if ((seekVec).GetMagnitude() > 0.00001f)
		{
			desiredVelocity = desiredVelocity + seekVec.Normalize() / (seekVec).GetMagnitude();
		}
		
		if (desiredVelocity.GetMagnitude() < maxDesiredVelocity)
		{
			
		}
		

		//desiredVelocity = desiredVelocity + behaviours[BEHAVIOUR_TYPE::SEEK]->PerformLogicAndGetVelocityVector();
		//desiredVelocity = desiredVelocity + behaviours[BEHAVIOUR_TYPE::FLEE]->PerformLogicAndGetVelocityVector();
		//if (!decisionMade)
		//{
		//	// Wander
		//}

		return desiredVelocity;
	}


private:
	float SEEK_DISTANCE_SQRD = 20000.0f * 20000.0f;
	float AVOID_DISTANCE_SQRD = 50.0f * 50.0f;

	float maxDesiredVelocity = 0.01f;
};

// Run away from closest target
class ScaredBrain : public Brain
{
public:

	ScaredBrain(PhysicsInfo in_self)
		:
		Brain(in_self)
	{
		decisionMade = false;
	}

	Vec2 DetermineDesiredVelocity(std::vector<PhysicsInfo> worldObjectPhysicsInfos) override
	{
		for (int j = 0; j < worldObjectPhysicsInfos.size(); j++)
		{
			worldObjectPhysicsInfos[j].SetRelativeInfo(self.loc, self.velocity);
		}

		std::sort(worldObjectPhysicsInfos.begin(), worldObjectPhysicsInfos.end());

		Vec2 desiredVelocity;

		for (int j = 0; j < worldObjectPhysicsInfos.size() && !decisionMade; j++)
		{
			if (self.ID != worldObjectPhysicsInfos[j].ID)
			{

				if (worldObjectPhysicsInfos[j].relativeDistSqrd < FLEE_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SEEKER)
				{
					// Flee
					behaviours[BEHAVIOUR_TYPE::FLEE]->AddTarget(worldObjectPhysicsInfos[j]);

					//decisionMade = true;
				}
				if (worldObjectPhysicsInfos[j].relativeDistSqrd < AVOID_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SCARED && worldObjectPhysicsInfos[j].isInEyesight)
				{
					// Flee
					behaviours[BEHAVIOUR_TYPE::FLEE]->AddTarget(worldObjectPhysicsInfos[j]);
				
					//decisionMade = true;
				}

				if (worldObjectPhysicsInfos[j].relativeDistSqrd < ALIGN_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SCARED && worldObjectPhysicsInfos[j].isInEyesight)
				{
					behaviours[BEHAVIOUR_TYPE::ALIGN]->AddTarget(worldObjectPhysicsInfos[j]);

					//decisionMade = true;
				}

				if (worldObjectPhysicsInfos[j].relativeDistSqrd < SEEK_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SCARED && worldObjectPhysicsInfos[j].isInEyesight)
				{
					behaviours[BEHAVIOUR_TYPE::SEEK]->AddTarget(worldObjectPhysicsInfos[j]);
					// Seek


					// Arrive
					//if (toTargetDist > 0)
					//{
					//	float speed = std::min(toTargetDist / 2.0f, maxSpeed);
					//	desiredVelocity = toTarget * speed / toTargetDist;
					//}

					//decisionMade = true;
				}

				
			}
		}

		Vec2 fleeVec = behaviours[BEHAVIOUR_TYPE::FLEE]->PerformLogicAndGetVelocityVector();
		Vec2 seekVec = behaviours[BEHAVIOUR_TYPE::SEEK]->PerformLogicAndGetVelocityVector();
		Vec2 alignVec = behaviours[BEHAVIOUR_TYPE::ALIGN]->PerformLogicAndGetVelocityVector();



		if ((alignVec).GetMagnitude() > 0.00001f)
		{
			desiredVelocity = desiredVelocity + (alignVec.Normalize() / (alignVec).GetMagnitude()) * FLEE_SCALE;
		}

		if ((fleeVec).GetMagnitude() > 0.00001f)
		{
			desiredVelocity = desiredVelocity + (fleeVec.Normalize() / (fleeVec).GetMagnitude()) * AVOID_SCALE;
		}

		if ((seekVec).GetMagnitude() > 0.00001f)
		{
			desiredVelocity = desiredVelocity + (seekVec.Normalize() / (seekVec).GetMagnitude()) * SEEK_SCALE;
		}



		
		
		//if (desiredVelocity.GetMagnitude() < 0.001f)
		//{
		//	desiredVelocity = behaviours[BEHAVIOUR_TYPE::FLEE]->PerformLogic();
		//}
		//if (!decisionMade)
		//{
		//	// Wander
		//}


		// Wander

		//randomness += (float)(rand() % 100) / 100.0f;
		//
		//float relAngle = 0.0f;
		//
		//if (desiredVelocity.GetMagnitude() < 0.1f)
		//{
		//	relAngle = (float)(rand() % 6282) / 1000.0f - 3.141f;
		//	if (self.velocity.GetMagnitude() < 5.0f)
		//	{
		//		float cosAngle = cos(relAngle);
		//		float sinAngle = sin(relAngle);
		//		desiredVelocity.x = 1.0f * sinAngle;
		//		desiredVelocity.y = 1.0f * cosAngle;
		//	}
		//}
		//else
		//{
		//	relAngle = acosf(desiredVelocity.y / desiredVelocity.GetMagnitude());
		//	//if (velocity.y < 0.0f)
		//	//{
		//	//	angle = 3.14159f - angle;
		//	//}
		//	if (desiredVelocity.x < 0.0f)
		//	{
		//		relAngle = -relAngle;
		//	}
		//}
		//
		//float cosAngle = cos(relAngle);
		//float sinAngle = sin(relAngle);
		//
		//float randSideways = (float)(sin(randomness)) / 1.0f;
		//
		//Vec2 sidewaysLoc = Vec2(randSideways, 1.0f);
		//Vec2 relativeLoc = Vec2();
		//
		//relativeLoc.x = (sidewaysLoc).x * cosAngle + (sidewaysLoc).y * sinAngle;
		//relativeLoc.y = (sidewaysLoc).y * cosAngle - (sidewaysLoc).x * sinAngle;
		//
		//desiredVelocity = desiredVelocity + (relativeLoc.Normalize()) / 1.0f;

		return desiredVelocity;
	}


private:
	float FLEE_DISTANCE_SQRD = 100.0f * 100.0f;
	float AVOID_DISTANCE_SQRD = 100.0f * 100.0f;
	float SEEK_DISTANCE_SQRD = 2000.0f * 2000.0f;
	float ALIGN_DISTANCE_SQRD = 2000.0f * 2000.0f;

	float FLEE_SCALE = 1.0f;
	float AVOID_SCALE = 1.0f;
	float SEEK_SCALE = 1.0f;
	float ALIGN_SCALE = 1.0f;

};