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
		// Remove relativeInfo from PhysicsInfo for mt... doesn't belong there anyway??

		std::vector<RelativeInfo> worldObjectRelativeInfos;

		for (int j = 0; j < worldObjectPhysicsInfos.size(); j++)
		{
			worldObjectRelativeInfos.push_back(RelativeInfo());
			worldObjectRelativeInfos[j].SetRelativeInfo(selfPtr, worldObjectPhysicsInfos[j]);
		}

		std::sort(worldObjectRelativeInfos.begin(), worldObjectRelativeInfos.end(), [](RelativeInfo &a, RelativeInfo &b) { return a < b; });


		for (int j = 0; j < worldObjectRelativeInfos.size(); j++)
		{
			int physicsInfoIndex = -1;
			for (int k = 0; k < worldObjectPhysicsInfos.size(); k++)
			{
				if (worldObjectPhysicsInfos[k]->ID == worldObjectRelativeInfos[j].ID)
				{
					physicsInfoIndex = k;
					break;
				}
				
			}
			if (selfPtr->ID != worldObjectPhysicsInfos[physicsInfoIndex]->ID && worldObjectRelativeInfos[j].isInEyesight)
			{
				for (int b = 0; b < behaviours.size(); b++)
				{
					if (worldObjectRelativeInfos[j].relativeDistSqrd > behaviours[b]->info.addTargetsInDistSqrdMin &&
						worldObjectPhysicsInfos[physicsInfoIndex]->brainType == behaviours[b]->info.targetBrainType)
					{
						if (behaviours[b]->info.currentTargets < behaviours[b]->info.maxTargets &&
							worldObjectRelativeInfos[j].relativeDistSqrd < behaviours[b]->info.addTargetsInDistSqrdMax)
						{
							behaviours[b]->AddTarget(worldObjectPhysicsInfos[physicsInfoIndex], worldObjectRelativeInfos[j]);
							behaviours[b]->info.currentTargets++;
						}
						else
						{
							break;
						}
					}
				}
			}
		}

		for (int b = 0; b < behaviours.size(); b++)
		{
			if (behaviours[b]->info.isActive)
			{
				behaviours[b]->PerformLogicAndSetVelocityVectors();

				// Think about this!!

				selfPtr->totalSteeringVelocity = selfPtr->totalSteeringVelocity + behaviours[b]->info.steeringVelocity * behaviours[b]->info.intensity;
			}

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

class SharkBrain : public Brain
{
public:

	SharkBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEEK GROUP", SEEK, BRAIN_TYPE::BEE, 0.0f, 60000.0f * 60000.0f, 1.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEEK SINGLE", SEEK, BRAIN_TYPE::BEE, 0.0f, 500.0f * 500.0f, 5.0f, 1 }));

		behaviours.push_back(CreateBehaviour(in_selfPtr, { "ARRIVE SINGLE", ARRIVE, BRAIN_TYPE::BEE, 0.0f, 10000.0f * 10000.0f, 10000.0f, 1 }));

		behaviours.push_back(CreateBehaviour(in_selfPtr, { "PURSUE SINGLE", PURSUE, BRAIN_TYPE::BEE, 0.0f, 10000.0f * 10000.0f, 10000.0f, 1 }));


		//behaviours.push_back(CreateBehaviour(in_selfPtr, { "WANDER", WANDER, BRAIN_TYPE::FISH, 0.0f, 0.0f, 1.0f, 0 }));
		//behaviours.push_back(CreateBehaviour(in_selfPtr, { "SLOW DOWN", SLOW_DOWN, BRAIN_TYPE::FISH, 0.0f, 0.0f, 1.0f, 0 }));
		//behaviours.push_back(CreateBehaviour(in_selfPtr, { "FLEE WALLS", FLEE, BRAIN_TYPE::NONE, 0.0f, 300.0f * 300.0f, 10000000.0f, 1 }));


	}

};



class FishBrain : public Brain
{
public:

	FishBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "ALIGN TO FISH", ALIGN, BRAIN_TYPE::FISH, 0.0f, 1000.0f * 1000.0f, 2.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEEK FAR FISH", SEEK, BRAIN_TYPE::FISH, 0.0f, 60000.0f * 60000.0f, 1.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEPARATE FISH", SEPARATION, BRAIN_TYPE::FISH, 0.0f, 200.0f * 200.0f, 3.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "FLEE SHARK", EVADE, BRAIN_TYPE::SHARK, 0.0f, 400.0f * 400.0f, 10.0f, 1 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "WANDER", WANDER, BRAIN_TYPE::FISH, 0.0f, 0.0f, 1.0f, 0 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SLOW DOWN", SLOW_DOWN, BRAIN_TYPE::FISH, 0.0f, 0.0f, 1.0f, 0 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "FLEE WALLS", FLEE, BRAIN_TYPE::NONE, 0.0f, 300.0f * 300.0f, 10000000.0f, 1 }));
	}


};


class BeeBrain : public Brain
{
public:

	BeeBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{
		//behaviours.push_back(CreateBehaviour(in_selfPtr, { "ALIGN TO FISH", ALIGN, BRAIN_TYPE::BEE, 0.0f, 1000.0f * 1000.0f, 0.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEEK FAR BEES", SEEK, BRAIN_TYPE::BEE, 400.0f * 400.0f, 60000.0f * 60000.0f, 5000.0f, 1000 }));
		//behaviours.push_back(CreateBehaviour(in_selfPtr, { "SEPARATION NEAR FISH", SEPARATION, BRAIN_TYPE::BEE, 0.0f, 500.0f * 500.0f, 0.0f, 10 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "FLEE SHARK", FLEE, BRAIN_TYPE::SHARK, 0.0f, 400.0f * 400.0f, 10000.0f, 1 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "FLEE FISH", FLEE, BRAIN_TYPE::FISH, 0.0f, 400.0f * 400.0f, 10000.0f, 1 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "WANDER", WANDER, BRAIN_TYPE::BEE, 0.0f, 0.0f, 1000.0f, 0 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SLOW DOWN", SLOW_DOWN, BRAIN_TYPE::BEE, 0.0f, 0.0f, 1.0f, 0 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "FLEE WALLS", FLEE, BRAIN_TYPE::NONE, 0.0f, 300.0f * 300.0f, 10000000.0f, 1 }));

	}


};


class ObstacleBrain : public Brain
{
public:

	ObstacleBrain(PhysicsInfo *in_selfPtr)
		:
		Brain(in_selfPtr)
	{
		// why slow down not working without seek/evade etc?
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "WANDER", WANDER, BRAIN_TYPE::BEE, 0.0f, 0.0f, 1000.0f, 1 }));
		behaviours.push_back(CreateBehaviour(in_selfPtr, { "SLOW DOWN", SLOW_DOWN, BRAIN_TYPE::BEE, 0.0f, 0.0f, 1.0f, 0 }));
	}
};



