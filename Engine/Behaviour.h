#pragma once

#include "PhysicsInfo.h"

class Behaviour
{
public:
	Behaviour(PhysicsInfo in_self)
		:
		self(in_self)
	{
	}

	void AddTarget(PhysicsInfo worldObjectPhysicsInfos)
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
				averageLoc = averageLoc + targets[i].loc;
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
				if (targets[i].relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / targets[i].relativeDistSqrd;
					averageLoc = averageLoc + targets[i].loc * invDist;
					totalInvDist += invDist;
				}
			}

			averageLoc = (averageLoc / totalInvDist);

		}

		return averageLoc;
	}

	Vec2 FindWeightedAverageVelocityOfTargetsByDistance()
	{
		Vec2 averageVelocity;

		if (targets.size() > 0)
		{
			float totalInvDist = 0.0f;
			for (int i = 0; i < targets.size(); i++)
			{
				if (targets[i].relativeDistSqrd > 0.1f)
				{
					float invDist = 1.0f / targets[i].relativeDistSqrd;
					averageVelocity = averageVelocity + targets[i].velocity * invDist;
					totalInvDist += invDist;
				}
			}

			averageVelocity = (averageVelocity / totalInvDist);

		}

		return averageVelocity;
	}

	virtual Vec2 PerformLogicAndGetVelocityVector() = 0;


protected:
	PhysicsInfo self;
	std::vector<PhysicsInfo> targets;
	BEHAVIOUR_TYPE behaviourType;

};

class BehaviourSeek : public Behaviour
{
public:
	BehaviourSeek(PhysicsInfo in_self)
		:
		Behaviour(in_self)
	{
		behaviourType = BEHAVIOUR_TYPE::SEEK;
	}

	

	Vec2 PerformLogicAndGetVelocityVector()
	{
		if (targets.size() > 0)
		{
			Vec2 averageLoc = FindWeightedAverageLocOfTargetsByDistance();

			return (averageLoc - self.loc);
		}
		else
		{
			return Vec2();
		}
	}


private:


};

class BehaviourFlee : public Behaviour
{
public:
	BehaviourFlee(PhysicsInfo in_self)
		:
		Behaviour(in_self)
	{
		behaviourType = BEHAVIOUR_TYPE::FLEE;
	}


	Vec2 PerformLogicAndGetVelocityVector()
	{
		if (targets.size() > 0)
		{
			Vec2 averageLoc = FindWeightedAverageLocOfTargetsByDistance();

			return (self.loc - averageLoc);
		}
		else
		{
			return Vec2();
		}
		
	}


private:


};

class BehaviourAlign : public Behaviour
{
public:
	BehaviourAlign(PhysicsInfo in_self)
		:
		Behaviour(in_self)
	{
		behaviourType = BEHAVIOUR_TYPE::ALIGN;
	}


	Vec2 PerformLogicAndGetVelocityVector()
	{
		if (targets.size() > 0)
		{
			Vec2 averageVelocity = FindWeightedAverageVelocityOfTargetsByDistance();

			return (averageVelocity);
		}
		else
		{
			return Vec2();
		}

	}


private:


};


static Behaviour* CreateBehaviour(PhysicsInfo self, BEHAVIOUR_TYPE type)
{
	switch (type)
	{
	case BEHAVIOUR_TYPE::SEEK:
	{
		return new BehaviourSeek(self);
	}break;
	case BEHAVIOUR_TYPE::FLEE:
	{
		return new BehaviourFlee(self);
	}break;
	case BEHAVIOUR_TYPE::ALIGN:
	{
		return new BehaviourAlign(self);
	}break;
	}
}