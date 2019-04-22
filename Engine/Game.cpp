/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	int maxObjects = 200;

	for (int i = 0; i < maxObjects; i++)
	{
		Color newColor = Colors::White;
		float newMaxSpeed = 0.0f;

		BRAIN_TYPE brainType = BRAIN_TYPE::SCARED; // (BRAIN_TYPE)(rand() % 2);
		
		if (brainType == BRAIN_TYPE::SEEKER)
		{
			newMaxSpeed = 50.0f;
			newColor = Colors::Red;
		}
		if (brainType == BRAIN_TYPE::SCARED)
		{
			newMaxSpeed = 500.0f;
			newColor = Colors::Green;
		}

		worldObjects.push_back(WorldObject(PhysicsInfo(brainType, Vec2((float)(rand() % (gfx.ScreenWidth)), (float)(rand() % (gfx.ScreenHeight))), Vec2(0.0f, 0.0f), worldObjects.size()), 1.0f, newMaxSpeed, newColor));// (float)(rand() % 100) - 50.0f, (float)(rand() % 100) - 50.0f), 1.0f)
	}

	//worldObjects.push_back(WorldObject(PhysicsInfo(BRAIN_TYPE::SEEKER, Vec2((float)(rand() % (gfx.ScreenWidth)), (float)(rand() % (gfx.ScreenHeight))), Vec2(0.0f, 0.0f), worldObjects.size()), 1.0f, 500.0f, Colors::Red));// (float)(rand() % 100) - 50.0f, (float)(rand() % 100) - 50.0f), 1.0f)

	
	cameraLoc = Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));
	cameraZoom = 1.0f;
}

void Game::Go()
{
	gfx.BeginFrame();	
	ProcessInput();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	float camMovement = 20.0f;

	float cosAngle = cos(relAngle);
	float sinAngle = sin(relAngle);

	if (wnd.kbd.KeyIsPressed('W'))
	{
		cameraLoc.x += camMovement * sinAngle;
		cameraLoc.y -= camMovement * cosAngle;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		cameraLoc.x -= camMovement * sinAngle;
		cameraLoc.y += camMovement * cosAngle;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		cameraLoc.x -= camMovement * cosAngle;
		cameraLoc.y -= camMovement * sinAngle;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		cameraLoc.x += camMovement * cosAngle;
		cameraLoc.y += camMovement * sinAngle;
	}

	float camZoomMultiplier = 1.1f;
	if (wnd.kbd.KeyIsPressed('Z'))
	{
		cameraZoom *= camZoomMultiplier;
	}
	if (wnd.kbd.KeyIsPressed('X'))
	{
		cameraZoom /= camZoomMultiplier;
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		Color newColor = Colors::White;
		float newMaxSpeed = 0.0f;

		BRAIN_TYPE brainType = BRAIN_TYPE::SEEKER; //(BRAIN_TYPE)(rand() % 2);

		if (brainType == BRAIN_TYPE::SEEKER)
		{
			newMaxSpeed = 500.0f;
			newColor = Colors::Red;
		}
		if (brainType == BRAIN_TYPE::SCARED)
		{
			newMaxSpeed = 80.0f;
			newColor = Colors::Green;
		}

		worldObjects.push_back(WorldObject(PhysicsInfo(brainType, Vec2((float)(rand() % (gfx.ScreenWidth)), (float)(rand() % (gfx.ScreenHeight))), Vec2(0.0f, 0.0f), worldObjects.size()), 1.0f, newMaxSpeed, newColor));// (float)(rand() % 100) - 50.0f, (float)(rand() % 100) - 50.0f), 1.0f)
	}
}

void Game::UpdateModel()
{
	float dt = 1.0f / 60.0f; // assume 60 fps


	//for (int i = 0; i < worldObjects.size(); i++)
	//{
	//	worldObjects[i].ResetNeighbourInfo();
	//	for (int j = 0; j < worldObjects.size(); j++)
	//	{
	//		if (i != j)
	//		{
	//			worldObjects[i].CalculateNeighbourInfo(worldObjects[j]);
	//		}
	//	}
	//}

	

	std::vector<PhysicsInfo> worldObjectPhysicsInfos;

	for (int i = 0; i < worldObjects.size(); i++)
	{
		worldObjectPhysicsInfos.push_back(worldObjects[i].GetPhysicsInfo());
	}

	for (int i = 0; i < worldObjects.size(); i++)
	{
		worldObjects[i].DetermineAction(worldObjectPhysicsInfos);
	}


	for (int i = 0; i < worldObjects.size(); i++)
	{
		worldObjects[i].Update(dt);
	}
	
}

void Game::ComposeFrame()
{





	cameraLoc = worldObjects[0].GetPhysicsInfo().loc;
	
	
	
	if (worldObjects[0].GetPhysicsInfo().velocity.GetMagnitude() < 0.1f)
	{
		relAngle = (float)(rand() % 6282) / 1000.0f - 3.141f;
	}
	else
	{
		relAngle = acosf(worldObjects[0].GetPhysicsInfo().velocity.y / worldObjects[0].GetPhysicsInfo().velocity.GetMagnitude());
		//if (velocity.y < 0.0f)
		//{
		//	
		//}
		relAngle = 3.14159f - relAngle;
	
		if (worldObjects[0].GetPhysicsInfo().velocity.x < 0.0f)
		{
			relAngle = -relAngle;
		}
	}
	
	
	


	if (wnd.kbd.KeyIsPressed('Q'))
	{
		relAngle -= 0.01f;
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		relAngle += 0.01f;
	}

	float cosAngle = cos(relAngle);
	float sinAngle = sin(relAngle);


	// Draw gridlines
	for (int y = 0; y < gfx.ScreenHeight; y++)
	{
		for (int x = 0; x < gfx.ScreenWidth; x++)
		{
			Vec2 worldLoc = Vec2();

			worldLoc.x = cameraLoc.x + (-(float)(gfx.ScreenWidth / 2) + (float)x) * cameraZoom * cosAngle - (-(float)(gfx.ScreenHeight / 2) + (float)y) * cameraZoom * sinAngle;
			worldLoc.y = cameraLoc.y + (-(float)(gfx.ScreenHeight / 2) + (float)y) * cameraZoom * cosAngle + (-(float)(gfx.ScreenWidth / 2) + (float)x) * cameraZoom * sinAngle;

			int xCheck = abs((int)worldLoc.x);
			int yCheck = abs((int)worldLoc.y);

			if (xCheck % 500 == 0 || yCheck % 500 == 0
				|| xCheck % 500 == 1 || yCheck % 500 == 1
				|| xCheck % 500 == 2 || yCheck % 500 == 2
				|| xCheck % 500 == 3 || yCheck % 500 == 3)
			{
				gfx.PutPixel(x, y, Colors::Blue);
			}
		}
	}




	for (int i = 0; i < worldObjects.size(); i++)
	{
		Vec2 relativeLoc = Vec2();
		
		relativeLoc.x = (worldObjects[i].GetPhysicsInfo().loc - cameraLoc).x * cosAngle + (worldObjects[i].GetPhysicsInfo().loc - cameraLoc).y * sinAngle;
		relativeLoc.y = (worldObjects[i].GetPhysicsInfo().loc - cameraLoc).y * cosAngle - (worldObjects[i].GetPhysicsInfo().loc - cameraLoc).x * sinAngle;


		gfx.DrawCircle((relativeLoc) / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2)), worldObjects[i].GetRadius() / cameraZoom, worldObjects[i].GetColor());
	}


	

	// draw vectors!
}
