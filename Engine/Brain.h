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
		
	}


	void Refresh()
	{
		// Reset all behaviours
		for (int i = 0; i < behaviours.size(); i++)
		{
			behaviours[i]->targets.clear();
			behaviours[i]->allBehaviours = behaviours;

			//behaviours[i]->info.desiredVelocity = Vec2();
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

		std::sort(worldObjectPhysicsInfos.begin(), worldObjectPhysicsInfos.end(), [](PhysicsInfo *a, PhysicsInfo *b) { return *a < *b; });

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

			//selfPtr->totalDesiredVelocity = selfPtr->totalDesiredVelocity + behaviours[b]->info.desiredVelocity * behaviours[b]->info.intensity;
			
			selfPtr->totalSteeringVelocity = selfPtr->totalSteeringVelocity + behaviours[b]->info.steeringVelocity * behaviours[b]->info.intensity;
			
		}

		//if (selfPtr->totalDesiredVelocity.GetMagnitude() > selfPtr->maxSpeed)
		//{
		//	selfPtr->totalDesiredVelocity = selfPtr->totalDesiredVelocity.Normalize() * selfPtr->maxSpeed;
		//}
		if (selfPtr->totalSteeringVelocity.GetMagnitude() > selfPtr->maxSpeed)
		{
			selfPtr->totalSteeringVelocity = selfPtr->totalSteeringVelocity.Normalize() * selfPtr->maxSpeed;
		}

		selfPtr->totalDesiredVelocity = selfPtr->totalSteeringVelocity + selfPtr->velocity;

		if (selfPtr->ID == 0)
		{
			int test = 0;
		}

	}
	

	PhysicsInfo *selfPtr;
	std::vector<Behaviour*> behaviours;
	float randomness;
};





// REDO THESE CLASSES SO THAT THEY ARE JUST ALL IN BRAIN WITH FACTORY GENERATOR!!!!

//static void BrainFactory()
//{
//
//}

class SeekerBrain : public Brain
{
public:

	SeekerBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEEK GROUP FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 60000.0f * 60000.0f, 1.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEEK SINGLE FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 500.0f * 500.0f, 5.0f, 1 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "WANDER", WANDER, BRAIN_TYPE::SCARED, 0.0f, 0.0f, 1.0f, 0 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SLOW DOWN", SLOW_DOWN, BRAIN_TYPE::SCARED, 0.0f, 0.0f, 1.0f, 0 }));
	}

};



class ScaredBrain : public Brain
{
public:

	ScaredBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "ALIGN TO FISH", ALIGN, BRAIN_TYPE::SCARED, 0.0f, 1000.0f * 1000.0f, 2.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEEK FAR FISH", SEEK, BRAIN_TYPE::SCARED, 0.0f, 60000.0f * 60000.0f, 1.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEPARATION NEAR FISH", SEPARATION, BRAIN_TYPE::SCARED, 0.0f, 200.0f * 200.0f, 3.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "FLEE SHARK", FLEE, BRAIN_TYPE::SEEKER, 0.0f, 400.0f * 400.0f, 10.0f, 1 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "WANDER", WANDER, BRAIN_TYPE::SCARED, 0.0f, 0.0f, 1.0f, 0 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SLOW DOWN", SLOW_DOWN, BRAIN_TYPE::SCARED, 0.0f, 0.0f, 1.0f, 0 }));
	}


};
