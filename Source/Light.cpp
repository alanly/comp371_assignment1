#include "Light.h"


Light::Light(void)
{
	lColor = glm::vec3(1.f, 1.f, 1.f);
	lCoefficients = glm::vec3(0.f, 0.f, 0.02f);
}


Light::~Light(void)
{
}

glm::vec3 Light::GetLightColor()
{
	return lColor;
}

glm::vec3 Light::GetLightCoefficients()
{
	return lCoefficients;
}

glm::vec4 Light::GetLightPosition()
{
	return lPosition;
}
