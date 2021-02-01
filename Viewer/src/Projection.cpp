#include "Projection.h"

Projection::Projection(const float aspectRatio, const float near, const float far, ProjectionType projectionType) :
	aspectRatio(aspectRatio), zNear(near), zFar(far), projectionType(projectionType)
{

}

void Projection::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}

void Projection::SetNear(const float near)
{
	zNear = near;
}

void Projection::SetFar(const float far)
{
	zFar = far;
}

ProjectionType Projection::GetProjectionType()
{
	return projectionType;
}

float Projection::GetAspectRatio()
{
	return aspectRatio;
}

float Projection::GetNear()
{
	return zNear;
}

float Projection::GetFar()
{
	return zFar;
}