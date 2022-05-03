#pragma once
#include"ObjectBuffer.h"

class Triangle : public ObjectBuffer
{
public:
	Triangle()
	{
		float points[][6] = { 
			{ -1.0f,-1.0f,-1.0f , 0.1f , 0.5f , 1.0f},
			{ 1.0f,-1.0f,-1.0f	 ,0.5f , 0.5f , 0.3f },
			{ -1.0f,1.0f,-1.0f	, 0.2f , 0.7f , 0.4f},
			{ 1.0f,1.0f,-1.0f	, 0.4f , 0.3f , 0.5f},
			{ -1.0f,-1.0f,1.0f	, 0.8f , 0.7f , 0.3f},
			{ 1.0f,-1.0f,1.0f	, 0.6f , 0.9f , 1.0f},
			{ -1.0f,1.0f,1.0f	, 0.34f , 0.45f , 0.65f },
			{ 1.0f,1.0f,1.0f	, 1.0f , 0.5f , 0.5f},
							  
		};
		unsigned int indices[] = { 0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
								 };
		Set(points, sizeof(float) * 6, std::size(points), indices, std::size(indices));
	}
};