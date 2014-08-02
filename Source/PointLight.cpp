#include "PointLight.h"


PointLight::PointLight(glm::vec3 position)
{
	lPosition = glm::vec4(position, 1.f);
}


PointLight::~PointLight(void)
{
}
