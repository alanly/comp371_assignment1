#pragma once

#include <GLM/glm.hpp>

class Light
{
public:
	Light(void);
	virtual ~Light(void);

	// Expressed as (r, g, b)
	glm::vec3 GetLightColor();

	// Expressed as (kc, kl, kq)
	glm::vec3 GetLightCoefficients();

	// Expressed as (x, y, z, w), wherein w=1 -> point light, w=0 -> directional light
	glm::vec4 GetLightPosition();

protected:
	glm::vec3 lColor;
	glm::vec3 lCoefficients;
	glm::vec4 lPosition;
};

