
/******************************************************************************************
*	Game code and amendments by s0lly													   *
*	https://www.youtube.com/channel/UCOyCXEB8NTx3Xjm8UptwsIg							   *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
******************************************************************************************/

#include "Creature.h"

void Creature::UpdateState(std::vector<Creature>& creatures, float TOO_CLOSE, float MAX_GOOD_DISTANCE, float GETTING_TOO_FAR, Beacon& beacon)
{
	// Update State

	std::vector<RelativeLocationInfo> relativeLocInfos = GetRelativeLocationInfos(*this, creatures);

	std::sort(relativeLocInfos.begin(), relativeLocInfos.end());

	int targetObject = -1;


	for (int j = 0; j < relativeLocInfos.size(); j++)
	{
		if ((ID != relativeLocInfos[j].objectID) && (relativeLocInfos[j].loc.y >= 0))
		{
			if (relativeLocInfos[j].distSqrd < (TOO_CLOSE * TOO_CLOSE))
			{
				state.type = STATE_TYPE::STATE_EVADE;
				targetObject = j;
				break;
				
			}
			else if (relativeLocInfos[j].distSqrd < (MAX_GOOD_DISTANCE * MAX_GOOD_DISTANCE))
			{
				state.type = STATE_TYPE::STATE_RANDOMWALK;
				break;
			}
			else if (relativeLocInfos[j].distSqrd < (GETTING_TOO_FAR * GETTING_TOO_FAR))
			{
				state.type = STATE_TYPE::STATE_SEEK;
				targetObject = j;

			}
			//else
			//{
			//	state.SetState(STATE_TYPE::STATE_RANDOMWALK);
			//	break;
			//}
		}
	}

	if (targetObject == -1)
	{
		state.type = STATE_TYPE::STATE_RANDOMWALK;
	}


	if (beacon.isOn)
	{
		//float beaconDist = (beacon.loc.x - loc.x) * (beacon.loc.x - loc.x) + (beacon.loc.y - loc.y) * (beacon.loc.y - loc.y);

		float cosDir = cos(direction);
		float sinDir = sin(direction);

		Vec2 newVec(
			(beacon.loc.x - loc.x) * cosDir + (beacon.loc.y - loc.y) * sinDir,
			(beacon.loc.x - loc.x) * sinDir - (beacon.loc.y - loc.y) * cosDir);

		targetObject = (int)relativeLocInfos.size();
		relativeLocInfos.push_back(RelativeLocationInfo(newVec, 0.0f, targetObject));

		state.type = STATE_TYPE::STATE_SEEK;
			
	}


	// Update direction, position, etc

	float turnAmount = 0.0314f * 24.0f;

	switch (state.type)
	{
		case STATE_TYPE::STATE_EVADE:
		{
			int newDir = 1;

			if (relativeLocInfos[targetObject].loc.x > 0)
			{
				newDir = -1;
			}
			else
			{
				newDir = 1;
			}

			direction += turnAmount * newDir;

			newDir = rand() % 7 - 3;
			direction += turnAmount * (float)newDir / 5.0f;
		}break;

		case STATE_TYPE::STATE_SEEK:
		{
			int newDir = 1;

			if (relativeLocInfos[targetObject].loc.x > 0)
			{
				newDir = 1;
			}
			else
			{
				newDir = -1;
			}

			direction += turnAmount * newDir;

			newDir = rand() % 7 - 3;
			direction += turnAmount * (float)newDir / 5.0f;

		}break;

		case STATE_TYPE::STATE_RANDOMWALK:
		{
			int newDir = rand() % 2;
			newDir = newDir == 0 ? -1 : newDir;
			direction += turnAmount * newDir;
		}break;
	}

	direction = ClampAngle(direction);
}


void Creature::Move()
{
	float moveAmount = 100.0f;

	loc.x += moveAmount * sin(direction);
	loc.y -= moveAmount * cos(direction);
}