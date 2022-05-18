#pragma once
#include<chrono>

#include"Window.h"
#include"Canvas2D.h"
#include"Canvas3D.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include"TexturedCube.h"

template<typename funcT, typename ... argTs>
auto Binder(funcT func, argTs ... args)
{
	return std::bind(func, args..., std::placeholders::_1);
}

struct POS_T
{
	int x_pos = 0, y_pos = 0;
	int img_xpos = 0, img_ypos = 0;
};
void keyboardPress(Canvas2D& cntx, Window::KeyBoard::EventT ev)
{
	if (ev.KEY_CODE == 'C')
	{
		cntx.Clear();
	}
}
void MouseRightPress(Canvas2D& cntx, Window& wnd)
{
	POS_T* const pos_t = static_cast<POS_T*>(wnd.UserSideDataPointer);
	pos_t->x_pos = wnd.mouse.GetX();
	pos_t->y_pos = wnd.mouse.GetY();
}
void MouseRightRelease(Canvas2D& cntx, Window& wnd)
{
	POS_T* const pos_t = static_cast<POS_T*>(wnd.UserSideDataPointer);
	pos_t->img_xpos += wnd.mouse.GetX() - pos_t->x_pos;
	pos_t->img_ypos += wnd.mouse.GetY() - pos_t->y_pos;
}
void MouseMove(Canvas2D& cntx, Image2D& img, Window& wnd)
{
	POS_T* const pos_t = static_cast<POS_T*>(wnd.UserSideDataPointer);
	if (wnd.mouse.IsLeftPressed())
	{
		auto [x, y] = wnd.mouse.GetXY();
		const auto half = wnd.GetWidth() / 2;
		auto x1 = x < half ? half + (half - x) : half - (x - half);
		cntx.DrawPixel(x, y, { 255 , 255 , 255 });
		cntx.DrawPixel(x1, y, { 255 , 255 , 255 });
		cntx.DrawOnWindow();
	}
	else if (wnd.mouse.IsRightPressed())
	{
		cntx.Clear();
		cntx.DrawImage(img, pos_t->img_xpos + wnd.mouse.GetX() - pos_t->x_pos, pos_t->img_ypos + wnd.mouse.GetY() - pos_t->y_pos);
		cntx.DrawOnWindow();
	}
}

void Func2D()
{

	Window wnd1("wnd1", 500, 700);
	Canvas2D cntx1{ wnd1 };

	Image2D img1(L"C:/Users/Aritra Maji/Downloads/93428795_p0.png");
	POS_T pst1;

	wnd1.UserSideDataPointer = &pst1;
	wnd1.keyboard.OnKeyPress = Binder(keyboardPress, std::ref(cntx1));
	wnd1.mouse.OnRightPress = Binder(MouseRightPress, std::ref(cntx1));
	wnd1.mouse.OnRightRelease = Binder(MouseRightRelease, std::ref(cntx1));
	wnd1.mouse.OnMove = Binder(MouseMove, std::ref(cntx1), std::ref(img1));

	cntx1.DrawImage(img1);
	cntx1.DrawOnWindow();

	while (Window::GetWindowCount())
	{
		Window::ProcessWindowEvents();
	}
}
void Func3D()
{
	Window wnd("Window", 900, 700);
	Canvas3D c3d(wnd);

	DirectX::XMMATRIX transform;
	ConstantBuffer cbuff(c3d, &transform, sizeof(transform));

	VertexShader TextureVShader(c3d, L"VertexShaderTex.cso", {

		{InputElemDesc::INPUT_FORMAT::FLOAT3 , "POSITION" , 0},
		{InputElemDesc::INPUT_FORMAT::FLOAT2 , "TEXCOORD" , 12u}

		}, cbuff);
	PixelShader TexturePShader(c3d, L"PixelShaderTex.cso");

	std::vector<Texture> textures;
	textures.reserve(5);
	textures.emplace_back(c3d, Image2D(L"D:/Textures/1.jpg"));
	textures.emplace_back(c3d, Image2D(L"D:/Textures/2.jpg"));
	textures.emplace_back(c3d, Image2D(L"D:/Textures/3.jpg"));
	textures.emplace_back(c3d, Image2D(L"D:/Textures/4.jpg"));
	textures.emplace_back(c3d, Image2D(L"D:/Textures/5.jpg"));

	c3d.SetShader(TextureVShader);

	std::vector<TexturedCube> tc;
	tc.reserve(100);

	for (auto i = 0; i < 100; ++i)
	{
		tc.emplace_back(c3d);
	}

	using clock = std::chrono::system_clock;

	clock::time_point tp = clock::now();
	std::chrono::duration<float> d;

	TexturePShader.SetTexture(textures[3]);
	c3d.SetShader(TexturePShader);

	int BoxPerTex = tc.size() / textures.size();

	float Near = 0.5f;
	float Far = 40.0f;

	float X_pos = 0.0f;
	float Y_pos = 0.0f;

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
		else if (ent.KEY_CODE == 'X')
		{
			X_pos += 0.05f;
		}
		else if (ent.KEY_CODE == 'C')
		{
			X_pos -= 0.05f;
		}
		else if (ent.KEY_CODE == 'V')
		{
			Y_pos += 0.05f;
		}
		else if (ent.KEY_CODE == 'B')
		{
			Y_pos -= 0.05f;
		}

	};

	while (wnd.IsOpen())
	{
		c3d.ClearCanvas();

		d = clock::now() - tp;
		auto tm = d.count();

		for (auto i = 0, j = 0; i < tc.size(); i += BoxPerTex, ++j)
		{
			TexturePShader.SetTexture(textures[j]);
			c3d.SetShader(TexturePShader);

			for (auto k = 0; k < BoxPerTex; ++k)
			{
				transform = DirectX::XMMatrixTranspose(tc[i + k].Update(tm) * DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, Near, Far) * DirectX::XMMatrixTranslation(X_pos, Y_pos, 0.0f));
				cbuff.Update(&transform, sizeof(transform));

				c3d.Draw(tc[i + k]);
			}
		}
		c3d.PresentOnScreen();
		wnd.ProcessEvents();
	}
}