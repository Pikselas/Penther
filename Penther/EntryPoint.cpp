#include"Window.h"
#include"Canvas3D.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include"Cube.h"
#include"Paper.h"

#include<chrono>

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd("Window" , 900 , 700 );
	Canvas3D c3d(wnd);

	DirectX::XMMATRIX transform;
	ConstantBuffer cbuff(c3d , &transform, sizeof(transform));
	
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


	std::vector<Cube> tgs;
	std::vector<Paper> pips;

	for (auto i = 0; i < 100; ++i)
	{
		tgs.emplace_back(c3d);
		pips.emplace_back(c3d);
	}

	float Near = 0.5f;
	float Far = 40.0f;

	wnd.keyboard.EnableKeyRepeat();

	wnd.keyboard.OnKeyPress = [&](Window::KeyBoard::EventT ent) {
		
		if (ent.KEY_CODE == 'W')
		{
			Near += 0.05f;
			Far += 0.05f;
			if (Near >= 40.0f)
			{
				Near = 40.0f;
				Far = 75.5f;
			}
		}
		else if (ent.KEY_CODE == 'S')
		{
			Near -= 0.05f;
			Far -= 0.05f;
			if (Near <= 0.1f)
			{
				Near = 0.1f;
				Far = 39.6f;
			}
		}
	
	};

	auto t = std::chrono::system_clock::now();
	std::chrono::duration<float> dp;

	while (Window::GetWindowCount())
	{
		c3d.ClearCanvas();
		
		dp = std::chrono::system_clock::now() - t;
		
		for (auto i = 0 ; i < 100 ; ++i)
		{
			c3d.SetShader(cubeVShader);
			c3d.SetShader(CubePShader);

			auto mtrx =  DirectX::XMMatrixTranspose(tgs[i].Update(dp.count()) * DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, Near, Far));
			cbuff.Update(&mtrx, sizeof(mtrx));
			c3d.Draw(tgs[i]);

			c3d.SetShader(paperVShader);
			c3d.SetShader(paperPShader);

			mtrx = DirectX::XMMatrixTranspose(pips[i].Update(dp.count()) * DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, Near, Far) );
			cbuff.Update(&mtrx, sizeof(mtrx));
			c3d.Draw(pips[i]);
		}
		
		c3d.PresentOnScreen();
		Window::ProcessWindowEvents();
	}
	return 0;
}