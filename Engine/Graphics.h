/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Vec2.h"

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line);
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x, y, z;		// position
		float u, v;			// texcoords
	};
public:
	Graphics(class HWNDKey& key);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel(int x, int y, int r, int g, int b)
	{
		PutPixel(x, y, { unsigned char(r),unsigned char(g),unsigned char(b) });
	}

	void DrawCircle(Vec2 loc, float radius, Color c)
	{
		int left = (int)(loc.x - radius);
		left = left < 0 ? 0 : left;

		int right = (int)(loc.x + radius) + 1;
		right = right >= ScreenWidth ? ScreenWidth : right;

		int top = (int)(loc.y - radius);
		top = top < 0 ? 0 : top;

		int bottom = (int)(loc.y + radius) + 1;
		bottom = bottom >= ScreenHeight ? ScreenHeight : bottom;

		float radiusSqrd = radius * radius;

		for (int j = top; j < bottom; j++)
		{
			float distSqrdY = ((float)j - loc.y) * ((float)j - loc.y);

			for (int i = left; i < right; i++)
			{
				float distSqrdX = ((float)i - loc.x) * ((float)i - loc.x);
				float distSqrdTotal = distSqrdX + distSqrdY;

				if (distSqrdTotal < radiusSqrd)
				{
					PutPixel(i, j, c);
				}
			}
		}
	}

	void DrawLine(Line line, Vec2 startLoc, Vec2 endLoc, Color c)
	{
		if (!line.reversedLine)
		{
			if (startLoc.x > endLoc.x)
			{
				std::swap(startLoc, endLoc);
			}
			for (int i = (int)startLoc.x; (int)i < endLoc.x; i++)
			{
				if (i >= 0 && i < ScreenWidth)
				{
					float yPos = line.GetY((float)i);
					if (yPos >= 0 && yPos < ScreenHeight)
					{
						PutPixel((int)i, (int)yPos, c);
					}
				}
			}
		}
		else
		{
			if (startLoc.y > endLoc.y)
			{
				std::swap(startLoc, endLoc);
			}
			for (int j = (int)startLoc.y; j < (int)endLoc.y; j++)
			{
				if (j >= 0 && j < ScreenHeight)
				{
					float xPos = line.GetX((float)j);
					if (xPos >= 0 && xPos < ScreenWidth)
					{
						PutPixel((int)xPos, (int)j, c);
					}
				}
			}
		}
	}

	//
	//void DrawVector()
	//{
	//
	//}

	void PutPixel( int x,int y,Color c );
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 1600;
	static constexpr int ScreenHeight = 900;
};