#pragma once
#include"ObjectBuffer.h"

class Triangle : public ObjectBuffer
{
public:
	Triangle(float x = 0.0f , float y = 0.0f)
	{
		float points[][6] = { 
							  {0.0f + x , 0.6f + y, 0.0f , 1.0f , 0.5f , 0.5f},
							  {0.5f + x , -0.5f + y , 0.0f , 0.5f , 1.0f , 0.5f},
							  {-0.5f+ x, -0.5f + y, 0.0f , 0.5f , 0.5f , 1.0f},
							  {0.5f + x , 0.3f + y , 0.0f ,0.2f , 0.4f , 0.7f},
							  {0.0f + x, -0.8f + y, 0.0f , 0.1f , 1.0f , 0.3f},
							  {-0.5 + x, 0.3f + y , 0.0f , 1.0f , 1.0f , 0.4f}
							  
		};
		unsigned int indices[] = { 0 , 1 , 2 ,
								   0 , 3 , 1 ,
								   1 , 4 , 2 ,
								   2 , 5 , 0
								 };
		Set(points, sizeof(float) * 6, std::size(points), indices, std::size(indices));
	}
};