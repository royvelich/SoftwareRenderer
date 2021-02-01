#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Model.h"

class GridModel : public Model
{
private:
	std::vector<glm::vec3> gridVertices;

public:
	GridModel(int sizeX, int sizeY, float unitSizeX, float unitSizeY);
	virtual ~GridModel();
};
