#pragma once
#include "light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight(glm::vec3 position);
	~DirectionalLight(void);
};

