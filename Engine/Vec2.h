
/******************************************************************************************
*	Game code and amendments by s0lly													   *
*	https://www.youtube.com/channel/UCOyCXEB8NTx3Xjm8UptwsIg							   *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
******************************************************************************************/

#pragma once

struct Vec2
{
	Vec2()
		:
		x(0.0f),
		y(0.0f)
	{
	}

	Vec2(float in_x, float in_y)
		:
		x(in_x),
		y(in_y)
	{
	}

	float x;
	float y;
};