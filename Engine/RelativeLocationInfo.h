#pragma once

#include "Vec2.h"

struct RelativeLocationInfo
{
	RelativeLocationInfo(Vec2 in_loc, float in_distSqrd, int in_ID)
		:
		loc(in_loc),
		distSqrd(in_distSqrd),
		objectID(in_ID)
	{
	}

	bool operator<(RelativeLocationInfo& rhs)
	{
		return distSqrd < rhs.distSqrd;
	}

	Vec2 loc;
	float distSqrd;
	int objectID;
};