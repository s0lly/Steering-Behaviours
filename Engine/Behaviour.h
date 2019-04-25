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

	void AddTarget(PhysicsInfo *in_worldObjectPhysicsInfo, RelativeInfo in_targetRelativeInfo)
	{
		targets.push_back(in_worldObjectPhysicsInfo);
		targetRelativeInfos.push_back(in_targetRelativeInfo);
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
				if (targetRelativeInfos[i].relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / targetRelativeInfos[i].relativeDistSqrd;
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
				if (targetRelativeInfos[i].relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / sqrt(targetRelativeInfos[i].relativeDistSqrd);
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
				if (targetRelativeInfos[i].relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / sqrt(targetRelativeInfos[i].relativeDistSqrd);
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
	std::vector<Behaviour*> allBehaviours;
	BehaviourInfo info;
	std::vector<RelativeInfo> targetRelativeInfos;

};

class BehaviourSeek : public Behaviour
{
public:
	BehaviourSeek(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		Behaviour(in_selfPtr, in_info)
	{
	}


	void PerformLogicAndSetVelocityVectors() override
	{
		if (targets.size() > 0)
		{
			Vec2 averageLoc = FindAverageLocOfTargets();

			Vec2 desiredVelocity = (averageLoc - selfPtr->loc).Normalize() * selfPtr->maxSpeed;
			info.steeringVelocity = desiredVelocity - selfPtr->velocity;
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


	void PerformLogicAndSetVelocityVectors() override
	{
		if (targets.size() > 0)
		{
			Vec2 averageLoc = FindAverageLocOfTargets();

			Vec2 desiredVelocity = (selfPtr->loc - averageLoc).Normalize() * selfPtr->maxSpeed;
			info.steeringVelocity = desiredVelocity - selfPtr->velocity;
		}
	}


private:


};

class BehaviourSeparation : public Behaviour
{
public:
	BehaviourSeparation(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		Behaviour(in_selfPtr, in_info)
	{
	}



	void PerformLogicAndSetVelocityVectors() override
	{
		if (targets.size() > 0)
		{
			for (int i = 0; i < targets.size(); i++)
			{
				Vec2 toTarget(selfPtr->loc - targets[i]->loc);

				info.steeringVelocity = info.steeringVelocity + (toTarget.Normalize() / toTarget.GetMagnitude()) * 1000.0f;
			}
			
			//info.desiredVelocity = info.steeringVelocity + selfPtr->velocity;
		}
	}


private:


};

class BehaviourAlign : public Behaviour
{
public:
	BehaviourAlign(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		Behaviour(in_selfPtr, in_info)
	{
	}

	void PerformLogicAndSetVelocityVectors() override
	{
		if (targets.size() > 0)
		{
			Vec2 averageVelocity = FindWeightedAverageVelocityOfTargetsByDistance();

			Vec2 desiredVelocity = (averageVelocity);
			info.steeringVelocity = desiredVelocity - selfPtr->velocity;
		}
	}


private:

};

class BehaviourWander : public Behaviour
{
public:
	BehaviourWander(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		Behaviour(in_selfPtr, in_info)
	{
	}

	void PerformLogicAndSetVelocityVectors() override
	{
		int totalNumTargets = 0;
		int totalMaxTargets = 0;

		for (int i = 0; i < allBehaviours.size(); i++)
		{
			totalNumTargets += allBehaviours[i]->info.currentTargets;
			totalMaxTargets += allBehaviours[i]->info.maxTargets;
		}

		float weightingToApply = totalNumTargets / totalMaxTargets;

		randomness += (float)(rand() % 100) / 1000.0f;

		float relAngle = 0.0f;

		if (selfPtr->velocity.GetMagnitude() < 0.1f)
		{
			relAngle = (float)(rand() % 6282) / 1000.0f - 3.141f;
			if (selfPtr->velocity.GetMagnitude() < 5.0f)
			{
				float cosAngle = cos(relAngle);
				float sinAngle = sin(relAngle);
			}
		}
		else
		{
			relAngle = acosf(selfPtr->velocity.y / selfPtr->velocity.GetMagnitude());

			if (selfPtr->velocity.x < 0.0f)
			{
				relAngle = -relAngle;
			}
		}

		float cosAngle = cos(relAngle);
		float sinAngle = sin(relAngle);

		float randSideways = (float)(sin(randomness));

		Vec2 sidewaysLoc = Vec2(randSideways, 0.5f + weightingToApply / 2.0f);
		Vec2 relativeLoc = Vec2();

		relativeLoc.x = (sidewaysLoc).x * cosAngle + (sidewaysLoc).y * sinAngle;
		relativeLoc.y = (sidewaysLoc).y * cosAngle - (sidewaysLoc).x * sinAngle;

		relativeLoc = (relativeLoc.Normalize()) * 1.0f * selfPtr->maxSpeed;

		info.steeringVelocity = relativeLoc;
	}


private:

	float randomness = (float)(rand() % 100);

};


class BehaviourSlowDown : public Behaviour
{
public:
	BehaviourSlowDown(PhysicsInfo *in_selfPtr, BehaviourInfo in_info)
		:
		Behaviour(in_selfPtr, in_info)
	{
	}

	void PerformLogicAndSetVelocityVectors() override
	{
		bool canPerformAction = true;

		for (int i = 0; i < allBehaviours.size(); i++)
		{
			if (allBehaviours[i]->info.currentTargets > 0)
			{
				canPerformAction = false;
			}
		}

		if (canPerformAction)
		{
			info.steeringVelocity = Vec2() - selfPtr->velocity;
		}
	}
};





static Behaviour* CreateBehaviour(PhysicsInfo *selfPtr, BehaviourInfo info)
{
	switch (info.type)
	{
	case BEHAVIOUR_TYPE::SEEK:
	{
		return new BehaviourSeek(selfPtr, info);
	}break;
	case BEHAVIOUR_TYPE::FLEE:
	{
		return new BehaviourFlee(selfPtr, info);
	}break;
	case BEHAVIOUR_TYPE::ALIGN:
	{
		return new BehaviourAlign(selfPtr, info);
	}break;
	case BEHAVIOUR_TYPE::SEPARATION:
	{
		return new BehaviourSeparation(selfPtr, info);
	}break;
	case BEHAVIOUR_TYPE::WANDER:
	{
		return new BehaviourWander(selfPtr, info);
	}break;
	case BEHAVIOUR_TYPE::SLOW_DOWN:
	{
		return new BehaviourSlowDown(selfPtr, info);
	}break;
	}
}

// Arrive
//if (toTargetDist > 0)
//{
//	float speed = std::min(toTargetDist / 2.0f, maxSpeed);
//	desiredVelocity = toTarget * speed / toTargetDist;
//}
