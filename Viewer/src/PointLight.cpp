#include "PointLight.h"
#include <algorithm>

PointLight::PointLight(glm::vec3 position) :
	Light(),
	position(position),
	scene(nullptr)
{
}

PointLight::PointLight(const std::shared_ptr<Scene>& scene) :
	Light(),
	scene(scene)
{

}

PointLight::~PointLight()
{
}

const glm::vec3 PointLight::CalculateIllumination(const glm::vec3& point, const glm::vec3& normal)
{
	if (scene)
	{
		position = scene->GetActiveCamera().GetEye();
	}
	return std::max(glm::dot(glm::normalize(position - point), normal), 0.0f) * color;
}