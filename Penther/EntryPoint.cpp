#include"Window.h"
#include"Canvas3D.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include"Cube.h"

#include<chrono>

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd("Window" , 900 , 700);
	Canvas3D c3d(wnd);

	DirectX::XMMATRIX transform;
	ConstantBuffer cbuff(c3d , &transform, sizeof(transform));

	c3d.SetShader(VertexShader( c3d,L"VertexShader.cso", { 
		
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "POSITION" , 0} ,
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "COLOR" , 12u}
		
		} , cbuff ));

	c3d.SetShader(PixelShader(c3d , L"PixelShader.cso"));


	std::vector<Cube> tgs;

	for (auto i = 0; i < 100; ++i)
	{
		tgs.emplace_back(c3d);
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
		for (auto& t : tgs)
		{
			auto mtrx =  DirectX::XMMatrixTranspose(t.Update(dp.count()) * DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, Near , Far));
			cbuff.Update(&mtrx, sizeof(mtrx));
			c3d.Draw(t);
		}
		c3d.PresentOnScreen();
		Window::ProcessWindowEvents();
	}
	return 0;
}