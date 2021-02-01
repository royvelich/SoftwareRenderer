#include "OrthographicProjection.h"

OrthographicProjection::OrthographicProjection(const float height, const float aspectRatio, const float near, const float far) :
	Projection(aspectRatio, near, far, ProjectionType::ORTHOGRAPHIC),
	height(height)
{

}

void OrthographicProjection::SetHeight(const float height)
{
	this->height = height;
}

const float OrthographicProjection::GetHeight()
{
	return height;
}

glm::mat4x4 OrthographicProjection::GenerateProjectionMatrix()
{
	return Utils::OrthographicProjectionMatrix(height, aspectRatio, zNear, zFar);
}