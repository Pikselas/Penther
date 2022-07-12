#pragma once
#pragma once
#include<random>
#include"Canvas3D.h"

class TexturedCube : public ObjectBuffer<Canvas3D>
{
private:
	struct VertexType
	{
		float x, y, z;
		float u , v;
	};
	static VertexType POINTS[];
	static const unsigned int INDICES[];
	static std::mt19937 engine;
	static std::uniform_real_distribution<float> rdist;
	static std::uniform_real_distribution<float> r2dist;
private:
	float p_roll, p_pitch, p_yaw;
	float s_roll, s_pitch, s_yaw;
	float r;
public:
	TexturedCube(const Canvas3D& c3d)
	{
		p_roll = rdist(engine);
		p_pitch = rdist(engine);
		p_yaw = rdist(engine);

		s_roll = rdist(engine);
		s_pitch = rdist(engine);
		s_yaw = rdist(engine);

		r = r2dist(engine);

		Set(c3d, POINTS, sizeof(VertexType), 24 , INDICES, 36);

	}
	DirectX::XMMATRIX Update(float dt)
	{
		using namespace DirectX;
		return (XMMatrixRotationRollPitchYaw(p_roll * dt, p_pitch * dt, p_yaw * dt) * XMMatrixTranslation(r, r, r) * XMMatrixRotationRollPitchYaw(s_roll * dt, s_pitch * dt, s_yaw * dt));
	}
};

TexturedCube::VertexType TexturedCube::POINTS[] = {
	 
	//front
	{-1.0f , 1.0f , -1.0f , 0.0f , 0.0f},
	{ 1.0f , 1.0f , -1.0f , 1.0f , 0.0f},
	{-1.0f , -1.0f , -1.0f , 0.0f , 1.0f},
	{ 1.0f , -1.0f , -1.0f , 1.0f , 1.0f},
	
	//left
	{-1.0f ,1.0f , 1.0f , 0.0f , 0.0f },
	{-1.0f ,1.0f , -1.0f , 1.0f , 0.0f },
	{-1.0f ,-1.0f , 1.0f , 0.0f , 1.0f },
	{-1.0f ,-1.0f , -1.0f , 1.0f , 1.0f },

	//top
	{-1.0f ,1.0f , 1.0f , 0.0f , 0.0f },
	{ 1.0f ,1.0f , 1.0f , 1.0f , 0.0f },
	{-1.0f ,1.0f , -1.0f , 0.0f , 1.0f },
	{ 1.0f ,1.0f , -1.0f , 1.0f , 1.0f },

	//back
	{-1.0f , 1.0f , 1.0f , 0.0f , 0.0f},
	{ 1.0f , 1.0f , 1.0f , 1.0f , 0.0f},
	{-1.0f , -1.0f , 1.0f , 0.0f , 1.0f},
	{ 1.0f , -1.0f , 1.0f , 1.0f , 1.0f},

	//right
	{1.0f ,1.0f , 1.0f , 0.0f , 0.0f },
	{1.0f ,1.0f , -1.0f , 1.0f , 0.0f },
	{1.0f ,-1.0f , 1.0f , 0.0f , 1.0f },
	{1.0f ,-1.0f , -1.0f , 1.0f , 1.0f },

	//bottom
	{-1.0f ,-1.0f , -1.0f , 0.0f , 0.0f },
	{ 1.0f ,-1.0f , -1.0f , 1.0f , 0.0f },
	{-1.0f ,-1.0f , 1.0f , 0.0f , 1.0f },
	{ 1.0f ,-1.0f , 1.0f , 1.0f , 1.0f },

};

const unsigned int TexturedCube::INDICES[] = {

	2 , 0 ,1,
	2 , 1 ,3,

	6 , 4, 5,
	5 , 7 ,6,

	10 , 8 , 9,
	9 , 11, 10,

	13 , 12 , 14,
	14 , 15 , 13,

	18 , 17 , 16,
	19 , 17 , 18,

	20 , 21 , 22,
	23 , 22 , 21

};

std::mt19937 TexturedCube::engine{ std::random_device{}() };
std::uniform_real_distribution<float> TexturedCube::rdist(-1.0f, 1.0f);
std::uniform_real_distribution<float> TexturedCube::r2dist(2.0f, 20.0);