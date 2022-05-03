#include"Window.h"
#include"Canvas3D.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include"Triangle.h"

int WINAPI wWinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE ,_In_ LPWSTR,_In_ int)
{
	Window wnd;
	Canvas3D c3d(wnd);

	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(90));

	ConstantBuffer cbuff(c3d , &transform, sizeof(transform));

	c3d.SetShader(VertexShader( c3d,L"VertexShader.cso", { 
		
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "POSITION" , 0} ,
		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "COLOR" , 12u}
		
		} , cbuff ));



	c3d.SetShader(PixelShader(c3d , L"PixelShader.cso"));

	Triangle t(0.0f , 0.0f);
	Triangle t2(0.5f, 0.5f);

	while (Window::GetWindowCount())
	{
		c3d.ClearCanvas();
		c3d.Draw(t);
		c3d.Draw(t2);
		c3d.PresentOnScreen();
		Window::ProcessWindowEvents();
	}
	return 0;
}