#pragma once

//static float TOO_CLOSE = 25.0f;
//static float MAX_GOOD_DISTANCE = 25.0f;
//static float GETTING_TOO_FAR = 10000.0f;

//static float TOO_CLOSE = 25.0f;
//static float MAX_GOOD_DISTANCE = 50.0f;
//static float GETTING_TOO_FAR = 400.0f;

enum STATE_TYPE
{
	STATE_EVADE,
	STATE_RANDOMWALK,
	STATE_SEEK
};

struct State
{
	STATE_TYPE type;
};