#pragma once

#include "PhysicsInfo.h"
#include <string>

class Behaviour
{
public:
	Behaviour(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		selfPtr(in_selfPtr),
		info(in_info)
	{
	}

	void AddTarget(PhysicsInfo *worldObjectPhysicsInfos)
	{
		targets.push_back(worldObjectPhysicsInfos);
	}

	Vec2 FindAverageLocOfTargets()
	{
		Vec2 averageLoc;

		if (targets.size() > 0)
		{

			for (int i = 0; i < targets.size(); i++)
			{
				averageLoc = averageLoc + targets[i]->loc;
			}

			averageLoc = (averageLoc / (float)targets.size());

		}

		return averageLoc;
	}

	Vec2 FindWeightedAverageLocOfTargetsByDistance()
	{
		Vec2 averageLoc;

		if (targets.size() > 0)
		{
			float totalInvDist = 0.0f;
			for (int i = 0; i < targets.size(); i++)
			{
				if (targets[i]->relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / targets[i]->relativeDistSqrd;
					averageLoc = averageLoc + targets[i]->loc * invDist;
					totalInvDist += invDist;
				}
			}

			averageLoc = (averageLoc / totalInvDist);

		}

		return averageLoc;
	}

	Vec2 FindCumulativeLocOfTargetsByDistance()
	{
		Vec2 cumuLoc;

		if (targets.size() > 0)
		{
			for (int i = 0; i < targets.size(); i++)
			{
				if (targets[i]->relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / sqrt(targets[i]->relativeDistSqrd);
					cumuLoc = cumuLoc + (targets[i]->loc - selfPtr->loc) * invDist;
				}
			}

		}

		return cumuLoc;
	}

	Vec2 FindWeightedAverageVelocityOfTargetsByDistance()
	{
		Vec2 averageVelocity;

		if (targets.size() > 0)
		{
			float totalInvDist = 0.0f;
			for (int i = 0; i < targets.size(); i++)
			{
				if (targets[i]->relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / targets[i]->relativeDistSqrd;
					averageVelocity = averageVelocity + targets[i]->velocity * invDist;
					totalInvDist += invDist;
				}
			}

			averageVelocity = (averageVelocity / totalInvDist);

		}

		return averageVelocity;
	}

	virtual void PerformLogicAndSetVelocityVectors() = 0;


//protected:
	PhysicsInfo *selfPtr;
	std::vector<PhysicsInfo*> targets;
	BehaviourInfo info;

};

class BehaviourSeek : public Behaviour
{
public:
	BehaviourSeek(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		Behaviour(in_selfPtr, in_info)
	{
	}


	void PerformLogicAndSetVelocityVectors()
	{
		if (targets.size() > 0)
		{
			Vec2 averageLoc = FindAverageLocOfTargets();

			info.desiredVelocity = (averageLoc - selfPtr->loc).Normalize() * selfPtr->maxSpeed;
			info.steeringVelocity = info.desiredVelocity - selfPtr->velocity;
		}
	}


private:


};


class BehaviourFlee : public Behaviour
{
public:
	BehaviourFlee(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		Behaviour(in_selfPtr, in_info)
	{
	}


	void PerformLogicAndSetVelocityVectors()
	{
		if (targets.size() > 0)
		{
			Vec2 averageLoc = FindAverageLocOfTargets();

			info.desiredVelocity = (selfPtr->loc - averageLoc).Normalize() * selfPtr->maxSpeed;
			info.steeringVelocity = info.desiredVelocity - selfPtr->velocity;
		}
	}


private:


};

//class BehaviourSeparation : public Behaviour
//{
//public:
//	BehaviourSeparation(PhysicsInfo in_self)
//		:
//		Behaviour(in_self)
//	{
//		behaviourType = BEHAVIOUR_TYPE::SEPARATION;
//	}
//
//
//
//	Vec2 PerformLogicAndGetVelocityVectors()
//	{
//		resultantVector = Vec2();
//
//		if (targets.size() > 0)
//		{
//			Vec2 cumuLoc = FindCumulativeLocOfTargetsByDistance();
//
//			resultantVector = (cumuLoc - self.loc);
//		}
//
//		return resultantVector;
//	}
//
//
//private:
//
//
//};
//
//class BehaviourCohesion : public Behaviour
//{
//public:
//	BehaviourCohesion(PhysicsInfo in_self)
//		:
//		Behaviour(in_self)
//	{
//		behaviourType = BEHAVIOUR_TYPE::SEEK;
//	}
//
//
//
//	Vec2 PerformLogicAndGetVelocityVectors()
//	{
//		resultantVector = Vec2();
//
//		if (targets.size() > 0)
//		{
//			Vec2 averageLoc = FindAverageLocOfTargets();
//
//			resultantVector = (averageLoc - self.loc);
//		}
//
//		return resultantVector;
//	}
//
//
//private:
//
//
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//class BehaviourAlign : public Behaviour
//{
//public:
//	BehaviourAlign(PhysicsInfo in_self)
//		:
//		Behaviour(in_self)
//	{
//		behaviourType = BEHAVIOUR_TYPE::ALIGN;
//	}
//
//
//	Vec2 PerformLogicAndGetVelocityVectors()
//	{
//		resultantVector = Vec2();
//
//		if (targets.size() > 0)
//		{
//			Vec2 averageVelocity = FindWeightedAverageVelocityOfTargetsByDistance();
//
//			resultantVector = averageVelocity;
//		}
//
//		return resultantVector;
//	}
//
//
//private:
//
//
//};


//static Behaviour* CreateBehaviour(PhysicsInfo self, BEHAVIOUR_TYPE type)
//{
//	switch (type)
//	{
//	case BEHAVIOUR_TYPE::SEEK:
//	{
//		return new BehaviourSeek(self);
//	}break;
//	//case BEHAVIOUR_TYPE::FLEE:
//	//{
//	//	return new BehaviourFlee(self);
//	//}break;
//	//case BEHAVIOUR_TYPE::ALIGN:
//	//{
//	//	return new BehaviourAlign(self);
//	//}break;
//	//case BEHAVIOUR_TYPE::COHESION:
//	//{
//	//	return new BehaviourCohesion(self);
//	//}break;
//	//case BEHAVIOUR_TYPE::SEPARATION:
//	//{
//	//	return new BehaviourSeparation(self);
//	//}break;
//	}
//}


