#include "AmbientLight.h"

AmbientLight::AmbientLight() :
	Light(glm::vec3(0.2, 0.2, 0.2))
{
}


AmbientLight::~AmbientLight()
{
}

const glm::vec3 AmbientLight::CalculateIllumination(const glm::vec3& point, const glm::vec3& normal)
{
	return color;
}