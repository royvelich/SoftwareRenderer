#include "GridModel.h"

GridModel::GridModel(int sizeX, int sizeY, float unitSizeX, float unitSizeY) :
	Model(GeometryType::LINES, "Grid")
{
	for (int i = -sizeX; i <= sizeX; i++)
	{
		float x = i * unitSizeX;
		float y = sizeY * unitSizeY;
		modelVertices.push_back(glm::vec3(x, 0, -y));
		modelVertices.push_back(glm::vec3(x, 0, y));
	}

	for (int i = -sizeY; i <= sizeY; i++)
	{
		float x = sizeX * unitSizeX;
		float y = i * unitSizeY;
		modelVertices.push_back(glm::vec3(-x, 0, y));
		modelVertices.push_back(glm::vec3(x, 0, y));
	}

	for (int i = 0; i < modelVertices.size(); i++)
	{
		modelNormals.push_back(glm::vec3(0, 0, 0));
	}
}

GridModel::~GridModel()
{

}