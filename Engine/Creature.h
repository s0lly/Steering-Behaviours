
/******************************************************************************************
*	Game code and amendments by s0lly													   *
*	https://www.youtube.com/channel/UCOyCXEB8NTx3Xjm8UptwsIg							   *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
******************************************************************************************/

#pragma once

#include "Colors.h"
#include "Vec2.h"
#include "State.h"
#include "RelativeLocationInfo.h"
#include "Beacon.h"
#include <vector>
#include <algorithm>
#include <random>
#include <functional>

static const float PI = 3.1415926f;

struct Creature
{
	void UpdateState(std::vector<Creature>& creatures, float TOO_CLOSE, float MAX_GOOD_DISTANCE, float GETTING_TOO_FAR, Beacon& beacon);

	void Move();

	Vec2 loc;
	float direction;
	float radius;
	Color color;
	State state;
	int ID;

	
};


static float ClampAngle(float angle)
{
	angle = angle < 0.0f ? angle + PI * 2.0f : angle;
	angle = angle >= PI * 2.0f ? angle - PI * 2.0f : angle;

	return angle;
}


static std::vector<RelativeLocationInfo> GetRelativeLocationInfos(const Creature& creature, const std::vector<Creature>& otherCreatures)
{
	std::vector<RelativeLocationInfo> newRelativeLocInfos;
	float cosDir = cos(creature.direction);
	float sinDir = sin(creature.direction);

	for (int i = 0; i < otherCreatures.size(); i++)
	{
		float distSqrd = 0.0f;
		
		//if ((creature.loc.x - otherCreatures[i].loc.x) < 10000.0f)
		//{
		//}
		//else
		//{
		//	distSqrd = 10000000.0f;
		//}

		distSqrd = (creature.loc.x - otherCreatures[i].loc.x) * (creature.loc.x - otherCreatures[i].loc.x) + (creature.loc.y - otherCreatures[i].loc.y) * (creature.loc.y - otherCreatures[i].loc.y);


		newRelativeLocInfos.push_back(
			RelativeLocationInfo(
				Vec2(
				(otherCreatures[i].loc.x - creature.loc.x) * cosDir + (otherCreatures[i].loc.y - creature.loc.y) * sinDir,
				(otherCreatures[i].loc.x - creature.loc.x) * sinDir - (otherCreatures[i].loc.y - creature.loc.y) * cosDir
				),
				distSqrd,
				otherCreatures[i].ID
			)
		);
	}

	return newRelativeLocInfos;
}