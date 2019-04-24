#pragma once

#include "Behaviour.h"
#include "PhysicsInfo.h"
#include "Vec2.h"
#include <vector>
#include <algorithm>


class Brain
{
public:
	Brain(PhysicsInfo *in_selfPtr)
		:
		selfPtr(in_selfPtr)
	{
		randomness = (float)(rand() % 100);
	}


	void Refresh()
	{
		// Reset all behaviours
		for (int i = 0; i < behaviours.size(); i++)
		{
			behaviours[i]->info.desiredVelocity = Vec2();
			behaviours[i]->info.steeringVelocity = Vec2();
			behaviours[i]->info.currentTargets = 0;
		}

		// Reset all total velocity infos
		selfPtr->totalDesiredVelocity = Vec2();
		selfPtr->totalSteeringVelocity = Vec2();
	}

	void DetermineVelocities(std::vector<PhysicsInfo*> worldObjectPhysicsInfos)
	{
		// Optimise required!! Sort on something else?? E.g. ID vector or similar. Or Pointers????? FIX!!!!
		for (int j = 0; j < worldObjectPhysicsInfos.size(); j++)
		{
			worldObjectPhysicsInfos[j]->SetRelativeInfo(selfPtr->loc, selfPtr->velocity);
		}

		//std::sort(worldObjectPhysicsInfos.begin(), worldObjectPhysicsInfos.end());

		for (int j = 0; j < worldObjectPhysicsInfos.size(); j++)
		{
			if (selfPtr->ID != worldObjectPhysicsInfos[j]->ID && worldObjectPhysicsInfos[j]->isInEyesight)
			{
				for (int b = 0; b < behaviours.size(); b++)
				{
					if (worldObjectPhysicsInfos[j]->relativeDistSqrd > behaviours[b]->info.addTargetsInDistSqrdMin &&
						worldObjectPhysicsInfos[j]->relativeDistSqrd < behaviours[b]->info.addTargetsInDistSqrdMax &&
						worldObjectPhysicsInfos[j]->brainType == behaviours[b]->info.targetBrainType &&
						behaviours[b]->info.currentTargets < behaviours[b]->info.maxTargets)
					{
						behaviours[b]->AddTarget(worldObjectPhysicsInfos[j]);
						behaviours[b]->info.currentTargets++;
					}
				}
			}
		}

		for (int b = 0; b < behaviours.size(); b++)
		{
			behaviours[b]->PerformLogicAndSetVelocityVectors();

			// Think about this!!

			selfPtr->totalDesiredVelocity = selfPtr->totalDesiredVelocity + behaviours[b]->info.desiredVelocity * behaviours[b]->info.intensity;
			
			selfPtr->totalSteeringVelocity = selfPtr->totalSteeringVelocity + behaviours[b]->info.steeringVelocity * behaviours[b]->info.intensity;
			
		}

		if (selfPtr->totalDesiredVelocity.GetMagnitude() > selfPtr->maxSpeed)
		{
			selfPtr->totalDesiredVelocity = selfPtr->totalDesiredVelocity.Normalize() * selfPtr->maxSpeed;
		}
		if (selfPtr->totalSteeringVelocity.GetMagnitude() > selfPtr->maxSpeed)
		{
			selfPtr->totalSteeringVelocity = selfPtr->totalSteeringVelocity.Normalize() * selfPtr->maxSpeed;
		}


		// Wander

		//randomness += (float)(rand() % 100) / 200.0f;
		//
		//float relAngle = 0.0f;
		//
		//if (self.desiredVelocity.GetMagnitude() < 0.1f)
		//{
		//	relAngle = (float)(rand() % 6282) / 1000.0f - 3.141f;
		//	if (self.velocity.GetMagnitude() < 5.0f)
		//	{
		//		float cosAngle = cos(relAngle);
		//		float sinAngle = sin(relAngle);
		//		self.desiredVelocity.x = 1.0f * sinAngle;
		//		self.desiredVelocity.y = 1.0f * cosAngle;
		//	}
		//}
		//else
		//{
		//	relAngle = acosf(self.desiredVelocity.y / self.desiredVelocity.GetMagnitude());
		//
		//	if (self.desiredVelocity.x < 0.0f)
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
		//self.desiredVelocity = self.desiredVelocity + (relativeLoc.Normalize()) * 100.0f;

	}
	


	

protected:
	PhysicsInfo *selfPtr;
	std::vector<Behaviour*> behaviours;
	float randomness;
	//std::vector<BehaviourInfo> behaviourInfos;
};





// REDO THESE CLASSES SO THAT THEY ARE JUST ALL IN BRAIN WITH FACTORY GENERATOR!!!!

class SeekerBrain : public Brain
{
public:

	SeekerBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{
		behaviours.push_back(new BehaviourSeek(in_selfPtr, { "SEEK GROUP FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 60000.0f * 60000.0f, 1.0f, 1 }));
		//behaviours.push_back(new BehaviourSeek(in_selfPtr, { "SEEK SINGLE FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 500.0f * 500.0f, 5.0f, 1 }));
		
		
		//behaviourInfos.push_back({ "SEEK GROUP FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 60000.0f * 60000.0f, 1.0f, 10 });
		//behaviourInfos.push_back({ "SEEK SINGLE FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 500.0f * 500.0f, 5.0f, 1 });

		//behaviourInfos.push_back({ "AVOID SHARKS", FLEE, BRAIN_TYPE::SEEKER, 0.0f, 1000.0f * 1000.0f, 0.1f, 9999 });
	}

};



class ScaredBrain : public Brain
{
public:

	ScaredBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{

		//behaviourInfos.push_back({ "COHESION FISH", COHESION, BRAIN_TYPE::SCARED, 0.0f, 20000.0f * 20000.0f, 3.0f, 9999 });
		//behaviourInfos.push_back({ "AVOID FISH", FLEE, BRAIN_TYPE::SCARED, 0.0f, 50.0f * 50.0f, 4.0f, 9999 });
		//behaviourInfos.push_back({ "ALIGN TO FISH", ALIGN, BRAIN_TYPE::SCARED, 0.0f, 100.0f * 100.0f, 0.05f, 9999 });
		
		//behaviours.push_back(new BehaviourSeek(in_selfPtr, {"SEEK FAR FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 60000.0f * 60000.0f, 1.0f, 9999 }));
		behaviours.push_back(new BehaviourFlee(in_selfPtr, { "AVOID NEAR FISH", FLEE, BRAIN_TYPE::SCARED, 0.0f, 100.0f * 100.0f, 1.0f, 9999 }));
		//behaviours.push_back(new BehaviourFlee(in_selfPtr, { "FLEE SHARK", FLEE, BRAIN_TYPE::SEEKER, 0.0f, 400.0f * 400.0f, 10.0f, 1 }));
	}


};















// Arrive
//if (toTargetDist > 0)
//{
//	float speed = std::min(toTargetDist / 2.0f, maxSpeed);
//	desiredVelocity = toTarget * speed / toTargetDist;
//}


	//PhysicsInfo DetermineDesiredVelocity(std::vector<PhysicsInfo> worldObjectPhysicsInfos) override
	//{
	//	for (int j = 0; j < worldObjectPhysicsInfos.size(); j++)
	//	{
	//		worldObjectPhysicsInfos[j].SetRelativeInfo(self.loc, self.velocity);
	//	}
	//
	//	std::sort(worldObjectPhysicsInfos.begin(), worldObjectPhysicsInfos.end());
	//
	//	self.desiredVelocity = Vec2();
	//
	//	for (int j = 0; j < worldObjectPhysicsInfos.size() && !decisionMade; j++)
	//	{
	//		if (self.ID != worldObjectPhysicsInfos[j].ID && worldObjectPhysicsInfos[j].isInEyesight)
	//		{
	//			for (int b = 0; b < behaviourInfos.size(); b++)
	//			{
	//				if (worldObjectPhysicsInfos[j].relativeDistSqrd < behaviourInfos[b].addTargetsInDistSqrd && worldObjectPhysicsInfos[j].brainType == behaviourInfos[b].targetBrainType)
	//				{
	//					behaviours[b]->AddTarget(worldObjectPhysicsInfos[j]);
	//				}
	//			}
	//		}
	//	}
	//
	//	for (int b = 0; b < behaviourInfos.size(); b++)
	//	{
	//		behaviourInfos[b].velocity = Vec2();
	//	}
	//
	//	for (int b = 0; b < behaviourInfos.size(); b++)
	//	{
	//		for (int t = 0; t < NUM_BEHAVIOURS; t++)
	//		{
	//			if (behaviourInfos[b].type == (BEHAVIOUR_TYPE)t)
	//			{
	//				behaviourInfos[b].velocity = behaviours[(BEHAVIOUR_TYPE)t]->PerformLogicAndGetVelocityVector() * behaviourInfos[b].intensity;
	//				self.desiredVelocity = self.desiredVelocity + behaviourInfos[b].velocity;
	//				self.behaviourInfos[b].velocity = behaviourInfos[b].velocity;
	//				break;
	//			}
	//		}
	//	}
	//
	//	//for (int j = 0; j < worldObjectPhysicsInfos.size() && !decisionMade; j++)
	//	//{
	//	//	if (self.ID != worldObjectPhysicsInfos[j].ID)
	//	//	{
	//	//
	//	//		if (worldObjectPhysicsInfos[j].relativeDistSqrd < FLEE_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SEEKER)
	//	//		{
	//	//			// Flee
	//	//			behaviours[BEHAVIOUR_TYPE::FLEE]->AddTarget(worldObjectPhysicsInfos[j]);
	//	//
	//	//			//decisionMade = true;
	//	//		}
	//	//		if (worldObjectPhysicsInfos[j].relativeDistSqrd < AVOID_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SCARED && worldObjectPhysicsInfos[j].isInEyesight)
	//	//		{
	//	//			// Flee
	//	//			behaviours[BEHAVIOUR_TYPE::FLEE]->AddTarget(worldObjectPhysicsInfos[j]);
	//	//		
	//	//			//decisionMade = true;
	//	//		}
	//	//
	//	//		if (worldObjectPhysicsInfos[j].relativeDistSqrd < ALIGN_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SCARED && worldObjectPhysicsInfos[j].isInEyesight)
	//	//		{
	//	//			behaviours[BEHAVIOUR_TYPE::ALIGN]->AddTarget(worldObjectPhysicsInfos[j]);
	//	//
	//	//			//decisionMade = true;
	//	//		}
	//	//
	//	//		if (worldObjectPhysicsInfos[j].relativeDistSqrd < SEEK_DISTANCE_SQRD && worldObjectPhysicsInfos[j].brainType == BRAIN_TYPE::SCARED && worldObjectPhysicsInfos[j].isInEyesight)
	//	//		{
	//	//			behaviours[BEHAVIOUR_TYPE::SEEK]->AddTarget(worldObjectPhysicsInfos[j]);
	//	//			// Seek
	//	//
	//	//
	//	//			// Arrive
	//	//			//if (toTargetDist > 0)
	//	//			//{
	//	//			//	float speed = std::min(toTargetDist / 2.0f, maxSpeed);
	//	//			//	desiredVelocity = toTarget * speed / toTargetDist;
	//	//			//}
	//	//
	//	//			//decisionMade = true;
	//	//		}
	//	//
	//	//		
	//	//	}
	//	//}
	//	//
	//	//self.desiredAlign = Vec2();
	//	//self.desiredFlee = Vec2();
	//	//self.desiredSeek = Vec2();
	//	//
	//	//Vec2 fleeVec = behaviours[BEHAVIOUR_TYPE::FLEE]->PerformLogicAndGetVelocityVector();
	//	//Vec2 seekVec = behaviours[BEHAVIOUR_TYPE::SEEK]->PerformLogicAndGetVelocityVector();
	//	//Vec2 alignVec = behaviours[BEHAVIOUR_TYPE::ALIGN]->PerformLogicAndGetVelocityVector();
	//	//
	//	//
	//	//
	//	//if ((alignVec).GetMagnitude() > 0.00001f)
	//	//{
	//	//	self.desiredAlign = (alignVec) * ALIGN_SCALE; //  / (alignVec).GetMagnitude()
	//	//	self.desiredVelocity = self.desiredVelocity + self.desiredAlign;
	//	//}
	//	//
	//	//if ((fleeVec).GetMagnitude() > 0.00001f)
	//	//{
	//	//	self.desiredFlee = (fleeVec / ((fleeVec).GetMagnitudeSqrd() / 1000.0f)) * AVOID_SCALE; //  
	//	//	self.desiredVelocity = self.desiredVelocity + self.desiredFlee;
	//	//}
	//	//
	//	//if ((seekVec).GetMagnitude() > 0.00001f)
	//	//{
	//	//	self.desiredSeek = (seekVec) * SEEK_SCALE; //  / (seekVec).GetMagnitude()
	//	//	self.desiredVelocity = self.desiredVelocity + self.desiredSeek;
	//	//}
	//
	//
	//
	//	
	//	
	//	//if (desiredVelocity.GetMagnitude() < 0.001f)
	//	//{
	//	//	desiredVelocity = behaviours[BEHAVIOUR_TYPE::FLEE]->PerformLogic();
	//	//}
	//	//if (!decisionMade)
	//	//{
	//	//	// Wander
	//	//}
	//
	//
	//	// Wander
	//
	//	randomness += (float)(rand() % 100) / 100.0f;
	//	
	//	float relAngle = 0.0f;
	//	
	//	if (self.desiredVelocity.GetMagnitude() < 0.1f)
	//	{
	//		relAngle = (float)(rand() % 6282) / 1000.0f - 3.141f;
	//		if (self.velocity.GetMagnitude() < 5.0f)
	//		{
	//			float cosAngle = cos(relAngle);
	//			float sinAngle = sin(relAngle);
	//			self.desiredVelocity.x = 1.0f * sinAngle;
	//			self.desiredVelocity.y = 1.0f * cosAngle;
	//		}
	//	}
	//	else
	//	{
	//		relAngle = acosf(self.desiredVelocity.y / self.desiredVelocity.GetMagnitude());
	//		//if (velocity.y < 0.0f)
	//		//{
	//		//	angle = 3.14159f - angle;
	//		//}
	//		if (self.desiredVelocity.x < 0.0f)
	//		{
	//			relAngle = -relAngle;
	//		}
	//	}
	//	
	//	float cosAngle = cos(relAngle);
	//	float sinAngle = sin(relAngle);
	//	
	//	float randSideways = (float)(sin(randomness)) / 1.0f;
	//	
	//	Vec2 sidewaysLoc = Vec2(randSideways, 1.0f);
	//	Vec2 relativeLoc = Vec2();
	//	
	//	relativeLoc.x = (sidewaysLoc).x * cosAngle + (sidewaysLoc).y * sinAngle;
	//	relativeLoc.y = (sidewaysLoc).y * cosAngle - (sidewaysLoc).x * sinAngle;
	//	
	//	self.desiredVelocity = self.desiredVelocity + (relativeLoc.Normalize()) * 10.0f;
	//
	//	return self;
	//}


	//PhysicsInfo DetermineDesiredVelocity(std::vector<PhysicsInfo> worldObjectPhysicsInfos) override
	//{
	//	for (int j = 0; j < worldObjectPhysicsInfos.size(); j++)
	//	{
	//		worldObjectPhysicsInfos[j].SetRelativeInfo(self.loc, self.velocity);
	//	}
	//
	//	std::sort(worldObjectPhysicsInfos.begin(), worldObjectPhysicsInfos.end());
	//
	//
	//	self.desiredVelocity = Vec2();
	//
	//	for (int j = 0; j < worldObjectPhysicsInfos.size() && !decisionMade; j++)
	//	{
	//		if (self.ID != worldObjectPhysicsInfos[j].ID && worldObjectPhysicsInfos[j].isInEyesight)
	//		{
	//			for (int b = 0; b < behaviourInfos.size(); b++)
	//			{
	//				if (worldObjectPhysicsInfos[j].relativeDistSqrd < behaviourInfos[b].addTargetsInDistSqrd && worldObjectPhysicsInfos[j].brainType == behaviourInfos[b].targetBrainType)
	//				{
	//					behaviours[b]->AddTarget(worldObjectPhysicsInfos[j]);
	//				}
	//			}
	//		}
	//	}
	//
	//	for (int b = 0; b < behaviourInfos.size(); b++)
	//	{
	//		behaviourInfos[b].velocity = Vec2();
	//	}
	//
	//	for (int b = 0; b < behaviourInfos.size(); b++)
	//	{
	//		for (int t = 0; t < NUM_BEHAVIOURS; t++)
	//		{
	//			if (behaviourInfos[b].type == (BEHAVIOUR_TYPE)t)
	//			{
	//				behaviourInfos[b].velocity = behaviours[(BEHAVIOUR_TYPE)t]->PerformLogicAndGetVelocityVector() * behaviourInfos[b].intensity;
	//				self.desiredVelocity = self.desiredVelocity + behaviourInfos[b].velocity;
	//				self.behaviourInfos[b].velocity = behaviourInfos[b].velocity;
	//				break;
	//			}
	//		}
	//	}
	//	
	//	return self;
	//}