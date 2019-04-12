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

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(100);

	Creature newCreature = {};

	int sizeOfField = 10000;

	for (int i = 0; i < numObjectsToAdd; i++)
	{
		creatures.push_back(newCreature);

		int fieldNum = rand() % 2;

		creatures[i].loc.x = (float)(rand() % sizeOfField - sizeOfField / 2); // fieldNum *     + sizeOfField * 4
		creatures[i].loc.y = (float)(rand() % sizeOfField - sizeOfField / 2);

		creatures[i].radius = 40.0f;

		creatures[i].direction = (((float)(rand() % 360)) / 360.0f) * PI * 2.0f;

		unsigned char newR = (unsigned char)(rand() % 256);
		unsigned char newG = 0;//(unsigned char)(rand() % 256);
		unsigned char newB = 0;//(unsigned char)(rand() % 256);
		creatures[i].color = Color(newR, newG, newB);

		//creatures[i].color = Colors::White;

		creatures[i].ID = i;
	}
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
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		cameraLoc.y += 100.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		cameraLoc.y -= 100.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		cameraLoc.x -= 100.0f;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		cameraLoc.x += 100.0f;
	}

	if (wnd.kbd.KeyIsPressed('A'))
	{
		cameraZoomOut *= 0.9f;
	}
	if (wnd.kbd.KeyIsPressed('Z'))
	{
		cameraZoomOut *= 1.1f;
	}

	if (wnd.mouse.LeftIsPressed())
	{
		

		float mouseRandomness = 0.0f;

		float fixedMouseX = wnd.mouse.GetPosX();
		float fixedMouseY = wnd.mouse.GetPosY();

		
		float newLocX = (fixedMouseX - (float)(gfx.ScreenWidth / 2)) * (cameraZoomOut) + cameraLoc.x + (((float)(rand() % 100)) / 50.0f - 1.0f) * mouseRandomness;
		float newLocY = -((fixedMouseY - (float)(gfx.ScreenHeight / 2)) * (cameraZoomOut) + cameraLoc.y) - (((float)(rand() % 100)) / 50.0f - 1.0f) * mouseRandomness;

		if (beacon.isOn)
		{
			beacon.loc.x = newLocX * 0.02f + beacon.loc.x * 0.98f;
			beacon.loc.y = newLocY * 0.02f + beacon.loc.y * 0.98f;
		}
		else
		{
			beacon.loc.x = newLocX;
			beacon.loc.y = newLocY;
		}
		
		beacon.strength = 5;

		beacon.isOn = true;
	}
	else
	{
		beacon.isOn = false;
	}
}

void Game::UpdateModel()
{
	//TOO_CLOSE = (float)(rand() % 1000);
	//MAX_GOOD_DISTANCE = TOO_CLOSE;
	//
	//TOO_CLOSE = 250.0f;
	//MAX_GOOD_DISTANCE = TOO_CLOSE;

	angle += 0.1f;

	float TOO_CLOSE = sin(angle) * 100.0f + 250.0f;
	float MAX_GOOD_DISTANCE = TOO_CLOSE;
	float GETTING_TOO_FAR = ((cos(angle) + 1.0f) / 2.0f) * ((2000.0f - MAX_GOOD_DISTANCE)) + MAX_GOOD_DISTANCE + 2000.0f;

	

	int numThreads = 20;
	int size = (int)creatures.size();
	
	int threadSize = size / numThreads + 1;
	
	auto creaturesPtr = &creatures;
	auto beaconPtr = &beacon;
	
	std::vector<std::thread> threadList;
	for (int k = 0; k < numThreads; k++)
	{
		threadList.push_back(std::thread([creaturesPtr, k, threadSize, TOO_CLOSE, MAX_GOOD_DISTANCE, GETTING_TOO_FAR, beaconPtr]()
		{
			
			for (int i = k * threadSize; (i < (k + 1) * threadSize) && (i < creaturesPtr->size()); i++)
			{
				(*creaturesPtr)[i].UpdateState(*creaturesPtr, TOO_CLOSE, MAX_GOOD_DISTANCE, GETTING_TOO_FAR, *beaconPtr);
			}
		}));
	}

	std::for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));


	//int numThreads = 20;
	//int size = (int)creatures.size();
	//
	//int threadSize = size / numThreads + 1;
	//
	//auto creaturesPtr = &creatures;
	//
	//for (int i = 0; i < creaturesPtr->size(); i++)
	//{
	//	(*creaturesPtr)[i].UpdateState(*creaturesPtr);
	//}


	for (int i = 0; i < creatures.size(); i++)
	{
		creatures[i].Move();
	}
}

void Game::ComposeFrame()
{
	if (beacon.isOn)
	{
		gfx.DrawCircle(Vec2((beacon.loc.x - cameraLoc.x) / (cameraZoomOut)+(float)(gfx.ScreenWidth / 2), -(beacon.loc.y - cameraLoc.y) / (cameraZoomOut)+(float)(gfx.ScreenHeight / 2)), 200.0f / (cameraZoomOut), Colors::Red);
	}

	for (auto& creature : creatures)
	{
		gfx.DrawCircle(Vec2((creature.loc.x - cameraLoc.x) / (cameraZoomOut)+(float)(gfx.ScreenWidth / 2), -(creature.loc.y - cameraLoc.y) / (cameraZoomOut)+(float)(gfx.ScreenHeight / 2)), creature.radius / (cameraZoomOut), creature.color);
	}
}
