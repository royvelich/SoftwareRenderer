#pragma once
#include "OrthographicProjection.h"
#include "PrespectiveProjection.h"
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 */
class Camera : public MeshModel
{
private:
	glm::mat4x4 cameraRotation;
	glm::mat4x4 cameraInverseRotation;
	glm::mat4x4 cameraTranslation;
	glm::mat4x4 cameraInverseTranslation;
	glm::mat4x4 cameraTransformation;
	glm::mat4x4 projectionTransformation;

	std::shared_ptr<Projection> projection;

	glm::vec3 eye;
	glm::vec3 up;
	glm::vec3 at;

	glm::vec3 f;
	glm::vec3 u;
	glm::vec3 l;

	float zoom;

public:
	Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up, const std::shared_ptr<Projection>& projection);
	~Camera();

	void SetCameraTransformation(const glm::mat4x4& transform);
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	void SetOrthographicProjection(
		const float height,
		const float aspectRatio,
		const float near,
		const float far);

	void SetPerspectiveProjection(
		const float fovy,
		const float aspect,
		const float near,
		const float far);

	void SetProjectionAspectRatio(const float aspectRatio);
	void SetZoom(const float zoom);

	void Zoom(const float factor);

	void SphericalRotate(const glm::vec2& sphericalDelta);

	const glm::mat4x4& GetProjectionTransformation() const;

	const glm::mat4x4& GetCameraTransformation() const;

	const std::shared_ptr<Projection>& GetProjection() const;

	void SetProjection(const std::shared_ptr<Projection>& projection);

	glm::vec4 GetEye();
};
