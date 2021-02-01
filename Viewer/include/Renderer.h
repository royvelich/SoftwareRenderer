#pragma once
#include "Scene.h"
#include "GeometryType.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>

class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;
	double viewportNear;
	double viewportFar;
	glm::mat4x4 viewportTransformation;

	void putPixel(int i, int j, const glm::vec3& color, float z, bool test_z = true);
	void drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);
	void inline putLinePixel(int x1, int y1, int dx, int dy, int deltaX, int deltaY, bool applySymmetry, const glm::vec3& color, double z);
	const glm::vec3 TransformVertex(const glm::vec4 vertex, const glm::mat4x4& world, const glm::mat4x4& view, const glm::mat4x4& proj);
	void createBuffers(int w, int h);
	GLuint glScreenTex;
	GLuint glScreenVtc;
	void createOpenGLBuffer();
	void initOpenGLRendering();
	void RenderModel(const Model& model, const Camera& camera, std::vector<std::shared_ptr<Light>> lights);
	void DrawLines(const std::vector<glm::vec3>& vertices, const glm::vec3& color, bool circular);
	void FillTriangle(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& worldVertices, const std::vector<glm::vec3>& normals, const glm::vec3& color, std::vector<std::shared_ptr<Light>> lights);
	double EdgeFunction(glm::vec3 v1, glm::vec3 v2, glm::vec3 p);
	double BarycentricCoordinates1D(const std::vector<glm::vec3>& vertices, const glm::vec3& p);
	glm::vec2 BarycentricCoordinates2D(const std::vector<glm::vec3>& vertices, const glm::vec3& p);

public:
	Renderer(int viewportWidth, int viewportHeight);
	~Renderer();

	void Render(const std::shared_ptr<Scene>& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int x, int y, int w, int h);
	const int GetViewportWidth() const;
	const int GetViewportHeight() const;
};
