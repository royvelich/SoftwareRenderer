#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Light.h"
#include "Scene.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position);
	PointLight(const std::shared_ptr<Scene>& scene);
	virtual ~PointLight();
	const glm::vec3 CalculateIllumination(const glm::vec3& point, const glm::vec3& normal);

private:
	glm::vec3 position;
	std::shared_ptr<Scene> scene;
};
