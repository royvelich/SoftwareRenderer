#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Model.h"
#include "Face.h"

class NormalModel : public Model
{
private:
	float normalLength;

public:
	NormalModel(std::vector<Face<3>> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, float normalLength);
	virtual ~NormalModel();
};
