#pragma once
#include<random>
#include"Canvas3D.h"

class Cube : public ObjectBuffer<Canvas3D>
{
private:
	struct VertexType
	{
		float x, y, z;
		float r, g, b;
	};
	static VertexType POINTS[];
	static const unsigned int INDICES[];
	static std::mt19937 engine;
	static std::uniform_real_distribution<float> rdist;
	static std::uniform_real_distribution<float> r2dist;
	static std::uniform_real_distribution<float> colorDist;
private:
	float p_roll, p_pitch, p_yaw;
	float s_roll, s_pitch, s_yaw;
	float r;
public:
	Cube(const Canvas3D& c3d)
	{

		std::mt19937 engine{ std::random_device{}() };
		std::uniform_real_distribution<float> rdist(-1.0f, 1.0f);
		std::uniform_real_distribution<float> r2dist(2.0f, 20.0);
		std::uniform_real_distribution colorDist(0.0f, 1.0f);

		p_roll = rdist(engine);
		p_pitch = rdist(engine);
		p_yaw = rdist(engine);

		s_roll = rdist(engine);
		s_pitch = rdist(engine);
		s_yaw = rdist(engine);

		r = r2dist(engine);

		for (auto i = 0; i < 8; ++i)
		{
			POINTS[i].r = colorDist(engine);
			POINTS[i].g = colorDist(engine);
			POINTS[i].b = colorDist(engine);
		}

		Set(c3d, POINTS, sizeof(VertexType) , 8, INDICES, 36);

	}
	DirectX::XMMATRIX Update(float dt)
	{
		using namespace DirectX;
		return (XMMatrixRotationRollPitchYaw(p_roll * dt , p_pitch * dt , p_yaw * dt) * XMMatrixTranslation(r , r , r) * XMMatrixRotationRollPitchYaw(s_roll * dt, s_pitch * dt, s_yaw * dt) * XMMatrixTranslation(0.0f, 0.0f, 20.0f));
	}
};

 Cube::VertexType Cube::POINTS[] = { 
			{ -1.0f,-1.0f,-1.0f , 0.1f , 0.5f , 1.0f},
			{ 1.0f,-1.0f,-1.0f	 ,0.5f , 0.5f , 0.3f },
			{ -1.0f,1.0f,-1.0f	, 0.2f , 0.7f , 0.4f},
			{ 1.0f,1.0f,-1.0f	, 0.4f , 0.3f , 0.5f},
			{ -1.0f,-1.0f,1.0f	, 0.8f , 0.7f , 0.3f},
			{ 1.0f,-1.0f,1.0f	, 0.6f , 0.9f , 1.0f},
			{ -1.0f,1.0f,1.0f	, 0.34f , 0.45f , 0.65f },
			{ 1.0f,1.0f,1.0f	, 1.0f , 0.5f , 0.5f}, };

 const unsigned int Cube::INDICES[] = { 
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4 };

 std::mt19937 Cube::engine{ std::random_device{}() };
 std::uniform_real_distribution<float> Cube::rdist(-1.0f, 1.0f);
 std::uniform_real_distribution<float> Cube::r2dist(2.0f, 20.0);
 std::uniform_real_distribution<float> Cube::colorDist(0.0f , 1.0f);