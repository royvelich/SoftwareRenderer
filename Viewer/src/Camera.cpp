#define _USE_MATH_DEFINES
#include <cmath>

#include "Camera.h"
#include "Utils.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up, const std::shared_ptr<Projection>& projection) :
	MeshModel(Utils::LoadMeshModel("../Data/camera.obj")),
	projection(projection),
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
	projectionTransformation = projection->GenerateProjectionMatrix();
}

Camera::~Camera()
{
}

void Camera::SetCameraTransformation(const glm::mat4x4& transform)
{
	cameraTransformation = transform;
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	this->eye = eye;
	this->at = at;
	this->up = up;

	f = glm::normalize(eye - at);
	l = glm::normalize(glm::cross(up, f));
	u = glm::cross(f, l);
	
	cameraRotation[0] = glm::vec4(l, 0);
	cameraRotation[1] = glm::vec4(u, 0);
	cameraRotation[2] = glm::vec4(f, 0);
	cameraRotation[3] = glm::vec4(0, 0, 0, 1);

	glm::mat4x4 cameraModelRotation;
	cameraModelRotation[0] = glm::vec4(-l, 0);
	cameraModelRotation[1] = glm::vec4(u, 0);
	cameraModelRotation[2] = glm::vec4(-f, 0);
	cameraModelRotation[3] = glm::vec4(0, 0, 0, 1);

	cameraInverseRotation = glm::transpose(cameraRotation);
	cameraTranslation = Utils::TranslationMatrix(eye);
	cameraInverseTranslation = Utils::TranslationMatrix(-eye);
	cameraTransformation = cameraInverseRotation * cameraInverseTranslation;
	worldTransform = cameraTranslation * cameraModelRotation * Utils::ScalingMatrix(glm::vec3(0.2,0.2,0.2));
}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{
	projection = std::make_unique<OrthographicProjection>(height, aspectRatio, near, far);
	projectionTransformation = projection->GenerateProjectionMatrix();
}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	projection = std::make_unique<PrespectiveProjection>(fovy, aspectRatio, near, far);
	projectionTransformation = projection->GenerateProjectionMatrix();
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projectionTransformation;
}

const glm::mat4x4& Camera::GetCameraTransformation() const
{
	return cameraTransformation;
}

void Camera::SetProjectionAspectRatio(const float aspectRatio)
{
	projection->SetAspectRatio(aspectRatio);
	projectionTransformation = projection->GenerateProjectionMatrix();
}

void Camera::SetZoom(const float zoom)
{
	this->zoom = zoom;
	projection->SetNear(projection->GetNear() * zoom);
	projectionTransformation = projection->GenerateProjectionMatrix();
}

void Camera::Zoom(const float factor)
{
	std::shared_ptr<PrespectiveProjection> prespectiveProjection = std::dynamic_pointer_cast<PrespectiveProjection>(projection);
	if (prespectiveProjection)
	{
		double newFovy = prespectiveProjection->GetFovy() * factor;
		if (newFovy > M_PI)
		{
			newFovy = M_PI;
		}

		prespectiveProjection->SetFovy(newFovy);
		projectionTransformation = prespectiveProjection->GenerateProjectionMatrix();
	}
}

void Camera::SphericalRotate(const glm::vec2& sphericalDelta)
{
	glm::mat4x4 vAxisRotation = Utils::AxisRotationMatrix(u, sphericalDelta.x);
	glm::mat4x4 uAxisRotation = Utils::AxisRotationMatrix(l, sphericalDelta.y);
	eye = uAxisRotation * vAxisRotation * glm::vec4(eye,1);
	SetCameraLookAt(eye, at, glm::vec3(0, 1, 0));
}

const std::shared_ptr<Projection>& Camera::GetProjection() const
{
	return projection;
}

void Camera::SetProjection(const std::shared_ptr<Projection>& projection)
{
	this->projection = projection;
	projectionTransformation = projection->GenerateProjectionMatrix();
}

glm::vec4 Camera::GetEye()
{
	return glm::vec4(eye, 1);
}