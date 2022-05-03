#include"Window.h"
#include"Canvas3D.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include"Triangle.h"

#include<chrono>

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd("Window" , 900 , 700);
	Canvas3D c3d(wnd);

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(90));

	ConstantBuffer cbuff(c3d , &transform, sizeof(transform));

	c3d.SetShader(VertexShader( c3d,L"VertexShader.cso", { 
		
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "POSITION" , 0} ,
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "COLOR" , 12u}
		
		} , cbuff ));

	c3d.SetShader(PixelShader(c3d , L"PixelShader.cso"));

	Triangle t1;
	Triangle t2;

	auto t = std::chrono::system_clock::now();
	std::chrono::duration<float> dp;


	while (Window::GetWindowCount())
	{
		c3d.ClearCanvas();
		auto angle = dp.count();
		dp = std::chrono::system_clock::now() - t;
		
		transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(angle) * DirectX::XMMatrixRotationX(angle) * DirectX::XMMatrixRotationY(angle) * DirectX::XMMatrixTranslation(0.0, 0.0, 4.0f)
			* DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10.0f));

		cbuff.Update(&transform, sizeof(transform));
		c3d.Draw(t1);

		transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(-angle) * DirectX::XMMatrixRotationX(-angle) * DirectX::XMMatrixRotationY(-angle) * DirectX::XMMatrixTranslation(0.0, 0.0, 7.0f)
			* DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.8, 10.0f));

		cbuff.Update(&transform, sizeof(transform));
		c3d.Draw(t1);

		c3d.PresentOnScreen();
		Window::ProcessWindowEvents();
	}
	return 0;
}