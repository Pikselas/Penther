#include"Window.h"
#include"Canvas3D.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include"Cube.h"
#include"Paper.h"
#include"TexturedCube.h"

#include<chrono>

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd("Window" , 900 , 700 );
	Canvas3D c3d(wnd);

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f , 5.0f) * DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 20.0f));
	ConstantBuffer cbuff(c3d , &transform, sizeof(transform));
	cbuff.Update(&transform , sizeof(transform));
	
	VertexShader cubeVShader(c3d, L"VertexShader.cso", {

		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "POSITION" , 0} ,
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "COLOR" , 12u}

		} , cbuff);
	PixelShader CubePShader (c3d , L"PixelShader.cso");

	VertexShader paperVShader(c3d, L"VertexShaderTex.cso",{
		
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "POSITION" , 0},
		{InputElemDesc::INPUT_FORMAT::FLOAT2 , "TEXCOORD" , 12u}
		
		}, cbuff);
	PixelShader paperPShader(c3d, L"PixelShaderTex.cso");

	Image2D img1(L"D:/CoderWallp/656567.jpg");
	Texture tex1(c3d , img1);

	paperPShader.SetTexture(tex1);

	c3d.SetShader(paperVShader);
	c3d.SetShader(paperPShader);

	TexturedCube tc(c3d);


	float x = 0.0f , y = 0.0f , z = 0.0f;
	wnd.keyboard.EnableKeyRepeat();
	wnd.keyboard.OnKeyPress = [&](Window::KeyBoard::EventT evnt) {

		switch (evnt.KEY_CODE)
		{
		case 'W':
			x += 0.05;
			break;
		case 'S':
			x -= 0.05;
			break;
		case 'A':
			y += 0.05;
			break;
		case 'D':
			y -= 0.05;
		}
		auto mtrx = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(x, y, z) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) * DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 20.0f));
		cbuff.Update(&mtrx, sizeof(mtrx));
	
	};


	while (wnd.IsOpen())
	{
		c3d.ClearCanvas();

		c3d.Draw(tc);

		c3d.PresentOnScreen();
		wnd.ProcessEvents();
	}
	return 0;
}