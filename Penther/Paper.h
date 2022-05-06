#pragma once
#include<random>
#include"Canvas3D.h"

class Paper : public ObjectBuffer<Canvas3D>
{
private:
	struct VertexType
	{
		float x, y , z;
		float u, v;
	};
private:
	static VertexType POINTS[];
	static const unsigned int INDICES[];
private:
	static std::mt19937 engine;
	static std::uniform_real_distribution<float> rdist;
	static std::uniform_real_distribution<float> r2dist;
private:
	float p_roll, p_pitch, p_yaw;
	float s_roll, s_pitch, s_yaw;
	float r;
public:
	Paper(const Canvas3D& c3d)
	{
		p_roll = rdist(engine);
		p_pitch = rdist(engine);
		p_yaw = rdist(engine);

		s_roll = rdist(engine);
		s_pitch = rdist(engine);
		s_yaw = rdist(engine);

		r = r2dist(engine);

		Set(c3d, POINTS, sizeof(VertexType), 8 , INDICES, 36);

	}
	DirectX::XMMATRIX Update(float dt)
	{
		using namespace DirectX;
		return (XMMatrixRotationRollPitchYaw(p_roll * dt, p_pitch * dt, p_yaw * dt) * XMMatrixTranslation(r, r, r) * XMMatrixRotationRollPitchYaw(s_roll * dt, s_pitch * dt, s_yaw * dt) * XMMatrixTranslation(0.0f, 0.0f, 20.0f));
	}
};

Paper::VertexType Paper::POINTS[] =
{
	{-1.0f , 1.0f , 1.0f , 0.0f , 0.0f},
	{1.0f , 1.0f , 1.0f  , 1.0f , 0.0f},
	{-1.0f , -1.0f , 1.0f, 0.0f , 1.0f },
	{1.0f , -1.0f , 1.0f , 1.0f , 1.0f}
};

const unsigned int Paper::INDICES[] =
{
	0 , 1 , 2,
	2 , 1 , 3
};

std::mt19937 Paper::engine{ std::random_device{}() };
std::uniform_real_distribution<float> Paper::rdist(-1.0f, 1.0f);
std::uniform_real_distribution<float> Paper::r2dist(2.0f, 20.0);