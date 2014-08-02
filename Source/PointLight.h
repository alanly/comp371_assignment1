#pragma once
#include "light.h"
class PointLight :
	public Light
{
public:
	PointLight(glm::vec3 position);
	~PointLight(void);
};

