#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(glm::vec3 position)
{
	lPosition = glm::vec4(position, 0.f);
}


DirectionalLight::~DirectionalLight(void)
{
}
