#include "PrespectiveProjection.h"

PrespectiveProjection::PrespectiveProjection(const float fovy, const float aspectRatio, const float near, const float far) :
	Projection(aspectRatio, near, far, ProjectionType::PRESPECTIVE),
	fovy(fovy)
{

}

void PrespectiveProjection::SetFovy(const float fovy)
{
	this->fovy = fovy;
}

const float PrespectiveProjection::GetFovy()
{
	return fovy;
}

glm::mat4x4 PrespectiveProjection::GenerateProjectionMatrix()
{
	return Utils::PrespectiveProjectionMatrix(fovy, aspectRatio, zNear, zFar);
}