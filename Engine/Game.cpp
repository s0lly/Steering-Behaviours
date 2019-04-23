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
	int maxObjects = 50;

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

	if (spaceIsActive || rightIsActive)
	{
		UpdateModel();
	}
	
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		if (!spaceIsPressed)
		{
			spaceIsPressed = true;
			spaceIsActive = !spaceIsActive;
		}
	}
	else
	{
		spaceIsPressed = false;
	}


	if (wnd.kbd.KeyIsPressed(VK_F1))
	{
		if (!f1IsPressed)
		{
			f1IsPressed = true;
			f1IsActive = !f1IsActive;
		}
	}
	else
	{
		f1IsPressed = false;
	}

	if (wnd.kbd.KeyIsPressed(VK_F2))
	{
		if (!f2IsPressed)
		{
			f2IsPressed = true;
			f2IsActive = !f2IsActive;
		}
	}
	else
	{
		f2IsPressed = false;
	}

	if (wnd.kbd.KeyIsPressed(VK_F3))
	{
		if (!f3IsPressed)
		{
			f3IsPressed = true;
			f3IsActive = !f3IsActive;
		}
	}
	else
	{
		f3IsPressed = false;
	}

	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		if (!rightIsPressed)
		{
			rightIsPressed = true;
			rightIsActive = true;
		}
		else
		{
			rightIsActive = false;
		}
	}
	else
	{
		rightIsPressed = false;
		rightIsActive = false;
	}




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

	if (wnd.kbd.KeyIsPressed(VK_RETURN))
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

	RetroContent::DrawString(gfx, "VECTORS", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5)), 6, Colors::Green);

	if (f1IsActive)
	{
		// Draw velocity vector

		Vec2 lineStart = Vec2() + cameraLoc;
		Vec2 lineEnd = worldObjects[0].GetPhysicsInfo().velocity + cameraLoc;

		Vec2 relativeLocStart = Vec2();
		relativeLocStart.x = (lineStart - cameraLoc).x * cosAngle + (lineStart - cameraLoc).y * sinAngle;
		relativeLocStart.y = (lineStart - cameraLoc).y * cosAngle - (lineStart - cameraLoc).x * sinAngle;

		relativeLocStart = relativeLocStart / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));

		Vec2 relativeLocEnd = Vec2();
		relativeLocEnd.x = (lineEnd - cameraLoc).x * cosAngle + (lineEnd - cameraLoc).y * sinAngle;
		relativeLocEnd.y = (lineEnd - cameraLoc).y * cosAngle - (lineEnd - cameraLoc).x * sinAngle;

		relativeLocEnd = relativeLocEnd / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));


		gfx.DrawLine(Line(relativeLocStart, relativeLocEnd), relativeLocStart, relativeLocEnd, Colors::Magenta);
		gfx.DrawCircle((relativeLocEnd), 5.0f / cameraZoom, Colors::Magenta);



		RetroContent::DrawString(gfx, "CURRENT", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 100.0f), 3, Colors::Magenta);
	}
	else
	{
		RetroContent::DrawString(gfx, "CURRENT", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 100.0f), 3, Colors::Gray);
	}



	if (f2IsActive)
	{
		// Draw desired velocity vector

		Vec2 line2Start = Vec2() + cameraLoc;
		Vec2 line2End = worldObjects[0].GetPhysicsInfo().desiredVelocity + cameraLoc;

		Vec2 relativeLoc2Start = Vec2();
		relativeLoc2Start.x = (line2Start - cameraLoc).x * cosAngle + (line2Start - cameraLoc).y * sinAngle;
		relativeLoc2Start.y = (line2Start - cameraLoc).y * cosAngle - (line2Start - cameraLoc).x * sinAngle;

		relativeLoc2Start = relativeLoc2Start / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));

		Vec2 relativeLoc2End = Vec2();
		relativeLoc2End.x = (line2End - cameraLoc).x * cosAngle + (line2End - cameraLoc).y * sinAngle;
		relativeLoc2End.y = (line2End - cameraLoc).y * cosAngle - (line2End - cameraLoc).x * sinAngle;

		relativeLoc2End = relativeLoc2End / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));


		gfx.DrawLine(Line(relativeLoc2Start, relativeLoc2End), relativeLoc2Start, relativeLoc2End, Colors::Cyan);
		gfx.DrawCircle((relativeLoc2End), 5.0f / cameraZoom, Colors::Cyan);

		RetroContent::DrawString(gfx, "DESIRED", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 150.0f), 3, Colors::Cyan);
	}
	else
	{
		RetroContent::DrawString(gfx, "DESIRED", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 150.0f), 3, Colors::Gray);
	}

	if (f3IsActive)
	{
		// Draw Align vector

		Vec2 lineAlignStart = Vec2() + cameraLoc;
		Vec2 lineAlignEnd = worldObjects[0].GetPhysicsInfo().desiredAlign + cameraLoc;

		Vec2 relativeLocAlignStart = Vec2();
		relativeLocAlignStart.x = (lineAlignStart - cameraLoc).x * cosAngle + (lineAlignStart - cameraLoc).y * sinAngle;
		relativeLocAlignStart.y = (lineAlignStart - cameraLoc).y * cosAngle - (lineAlignStart - cameraLoc).x * sinAngle;

		relativeLocAlignStart = relativeLocAlignStart / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));

		Vec2 relativeLocAlignEnd = Vec2();
		relativeLocAlignEnd.x = (lineAlignEnd - cameraLoc).x * cosAngle + (lineAlignEnd - cameraLoc).y * sinAngle;
		relativeLocAlignEnd.y = (lineAlignEnd - cameraLoc).y * cosAngle - (lineAlignEnd - cameraLoc).x * sinAngle;

		relativeLocAlignEnd = relativeLocAlignEnd / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));


		gfx.DrawLine(Line(relativeLocAlignStart, relativeLocAlignEnd), relativeLocAlignStart, relativeLocAlignEnd, Colors::White);
		gfx.DrawCircle((relativeLocAlignEnd), 5.0f / cameraZoom, Colors::White);

		

		// Draw Seek vector

		Vec2 lineSeekStart = Vec2() + cameraLoc;
		Vec2 lineSeekEnd = worldObjects[0].GetPhysicsInfo().desiredSeek + cameraLoc;

		Vec2 relativeLocSeekStart = Vec2();
		relativeLocSeekStart.x = (lineSeekStart - cameraLoc).x * cosAngle + (lineSeekStart - cameraLoc).y * sinAngle;
		relativeLocSeekStart.y = (lineSeekStart - cameraLoc).y * cosAngle - (lineSeekStart - cameraLoc).x * sinAngle;

		relativeLocSeekStart = relativeLocSeekStart / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));

		Vec2 relativeLocSeekEnd = Vec2();
		relativeLocSeekEnd.x = (lineSeekEnd - cameraLoc).x * cosAngle + (lineSeekEnd - cameraLoc).y * sinAngle;
		relativeLocSeekEnd.y = (lineSeekEnd - cameraLoc).y * cosAngle - (lineSeekEnd - cameraLoc).x * sinAngle;

		relativeLocSeekEnd = relativeLocSeekEnd / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));


		gfx.DrawLine(Line(relativeLocSeekStart, relativeLocSeekEnd), relativeLocSeekStart, relativeLocSeekEnd, Colors::Red);
		gfx.DrawCircle((relativeLocSeekEnd), 5.0f / cameraZoom, Colors::Red);

		

		// Draw Flee vector

		Vec2 lineFleeStart = Vec2() + cameraLoc;
		Vec2 lineFleeEnd = worldObjects[0].GetPhysicsInfo().desiredFlee + cameraLoc;

		Vec2 relativeLocFleeStart = Vec2();
		relativeLocFleeStart.x = (lineFleeStart - cameraLoc).x * cosAngle + (lineFleeStart - cameraLoc).y * sinAngle;
		relativeLocFleeStart.y = (lineFleeStart - cameraLoc).y * cosAngle - (lineFleeStart - cameraLoc).x * sinAngle;

		relativeLocFleeStart = relativeLocFleeStart / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));

		Vec2 relativeLocFleeEnd = Vec2();
		relativeLocFleeEnd.x = (lineFleeEnd - cameraLoc).x * cosAngle + (lineFleeEnd - cameraLoc).y * sinAngle;
		relativeLocFleeEnd.y = (lineFleeEnd - cameraLoc).y * cosAngle - (lineFleeEnd - cameraLoc).x * sinAngle;

		relativeLocFleeEnd = relativeLocFleeEnd / cameraZoom + Vec2((float)(gfx.ScreenWidth / 2), (float)(gfx.ScreenHeight / 2));


		gfx.DrawLine(Line(relativeLocFleeStart, relativeLocFleeEnd), relativeLocFleeStart, relativeLocFleeEnd, Colors::Yellow);
		gfx.DrawCircle((relativeLocFleeEnd), 5.0f / cameraZoom, Colors::Yellow);



		RetroContent::DrawString(gfx, "ALIGN", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 200.0f), 3, Colors::White);
		RetroContent::DrawString(gfx, "SEEK", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 250.0f), 3, Colors::Red);
		RetroContent::DrawString(gfx, "FLEE", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 300.0f), 3, Colors::Yellow);
	}
	else
	{
		RetroContent::DrawString(gfx, "ALIGN", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 200.0f), 3, Colors::Gray);
		RetroContent::DrawString(gfx, "SEEK", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 250.0f), 3, Colors::Gray);
		RetroContent::DrawString(gfx, "FLEE", Vec2((float)(200.0f), (float)(gfx.ScreenHeight * 3 / 5) + 300.0f), 3, Colors::Gray);
	}

}
