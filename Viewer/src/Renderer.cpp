#define _USE_MATH_DEFINES
#include <cmath>

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "EdgeTable.h"
#include <imgui/imgui.h>
#include <vector>
#include <memory>
#include <algorithm>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewportWidth, int viewportHeight) :
	viewportWidth(viewportWidth),
	viewportHeight(viewportHeight),
	viewportNear(0),
	viewportFar(1)
{
	initOpenGLRendering();
	createBuffers(viewportWidth, viewportHeight);
}

Renderer::~Renderer()
{
	delete[] colorBuffer;
	delete[] zBuffer;
}

void Renderer::putPixel(int i, int j, const glm::vec3& color, float z, bool test_z)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	if (!test_z || z < zBuffer[Z_INDEX(viewportWidth, i, j)])
	{
		colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
		colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
		colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
		zBuffer[Z_INDEX(viewportWidth, i, j)] = z;
	}
}

void Renderer::drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
{
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;
	int run;
	int rise;
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int AbsDeltaX = deltaX < 0 ? -deltaX : deltaX;
	int AbsDeltaY = deltaY < 0 ? -deltaY : deltaY;
	bool applySymmetry = AbsDeltaX < AbsDeltaY;

	if (applySymmetry)
	{
		run = AbsDeltaY;
		rise = AbsDeltaX;
	}
	else
	{
		run = AbsDeltaX;
		rise = AbsDeltaY;
	}

	int currentRun = 0;
	int currentRise = 0;
	int e = -run;
	int doubleRun = 2 * run;
	int doubleRise = 2 * rise;
	while (currentRun <= run)
	{
		if (e > 0)
		{
			e -= doubleRun;
			currentRise++;
		}

		double lambda = (p1.x - x1) / (x2 - x1);
		double z_reciprocal = (1 - lambda) * (1 / p1.z) + (lambda) * (1 / p2.z);
		double z = (1/z_reciprocal);

		putLinePixel(x1, y1, currentRun, currentRise, deltaX, deltaY, applySymmetry, color, z);

		currentRun++;
		e += doubleRise;
	}
}

void inline Renderer::putLinePixel(int x1, int y1, int dx, int dy, int deltaX, int deltaY, bool applySymmetry, const glm::vec3& color, double z)
{
	if (applySymmetry)
	{
		int temp = dx;
		dx = dy;
		dy = temp;
	}

	if (deltaX >= 0)
	{
		if (deltaY >= 0)
		{
			putPixel(x1 + dx, y1 + dy, color, z);
		}
		else
		{
			putPixel(x1 + dx, y1 - dy, color, z);
		}
	}
	else
	{
		if (deltaY >= 0)
		{
			putPixel(x1 - dx, y1 + dy, color, z);
		}
		else
		{
			putPixel(x1 - dx, y1 - dy, color, z);
		}
	}
}

void Renderer::createBuffers(int w, int h)
{
	createOpenGLBuffer(); //Do not remove this line.
	colorBuffer = new float[3 * w * h];
	zBuffer = new float[w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, color, std::numeric_limits<float>::max(), false);
		}
	}
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
	viewportX = x;
	viewportY = y;
	viewportWidth = width;
	viewportHeight = height;
	delete[] colorBuffer;
	delete[] zBuffer;
	createBuffers(width, height);

	viewportTransformation[0] = glm::vec4(viewportWidth / 2, 0, 0, 0);
	viewportTransformation[1] = glm::vec4(0, viewportHeight / 2, 0, 0);
	viewportTransformation[2] = glm::vec4(0, 0, (viewportFar - viewportNear) / 2, 0);
	viewportTransformation[3] = glm::vec4(viewportWidth / 2, viewportHeight / 2, (viewportFar + viewportNear) / 2, 1);
}

const glm::vec3 Renderer::TransformVertex(const glm::vec4 vertex, const glm::mat4x4& world, const glm::mat4x4& view, const glm::mat4x4& projection)
{
	glm::vec4 transformedVertex;
	transformedVertex = projection * view * world * vertex;
	transformedVertex = transformedVertex / transformedVertex.w;
	transformedVertex = viewportTransformation * transformedVertex;
	return glm::vec3(transformedVertex);
}

void Renderer::DrawLines(const std::vector<glm::vec3>& vertices, const glm::vec3& color, bool circular)
{
	if (vertices.size() > 1)
	{
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			drawLine(vertices[i], vertices[i + 1], color);
		}

		if (circular)
		{
			drawLine(vertices[vertices.size() - 1], vertices[0], color);
		}
	}
}

glm::vec2 Renderer::BarycentricCoordinates2D(const std::vector<glm::vec3>& vertices, const glm::vec3& p)
{
	glm::vec2 u = vertices[1] - vertices[0];
	glm::vec2 v = vertices[2] - vertices[0];
	glm::vec2 w = p - vertices[0];

	double lambda1 = (w.y * v.x - v.y * w.x) / (u.y * v.x - u.x * v.y);
	double lambda2 = (w.y - lambda1 * u.y) / v.y;

	return glm::vec2(lambda1, lambda2);
}

double Renderer::EdgeFunction(glm::vec3 v1, glm::vec3 v2, glm::vec3 p)
{
	glm::vec3 v = v2 - v1;
	glm::vec3 u = p - v1;
	return glm::cross(v, u).z;
}

void Renderer::FillTriangle(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& worldVertices, const std::vector<glm::vec3>& normals, const glm::vec3& color, std::vector<std::shared_ptr<Light>> lights)
{
	int min_x = std::min(vertices[0].x, std::min(vertices[1].x, vertices[2].x));
	int max_x = std::max(vertices[0].x, std::max(vertices[1].x, vertices[2].x));
	int min_y = std::min(vertices[0].y, std::min(vertices[1].y, vertices[2].y));
	int max_y = std::max(vertices[0].y, std::max(vertices[1].y, vertices[2].y));

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			glm::vec3 p = glm::vec3(x, y, 0);
			glm::vec2 barycentricCoords = BarycentricCoordinates2D(vertices, p);

			if(barycentricCoords[0] >= 0 && barycentricCoords[1] >= 0 && ((barycentricCoords[0] + barycentricCoords[1]) <= 1))
			{
				float lambda1 = 1 - barycentricCoords.x - barycentricCoords.y;
				float lambda2 = barycentricCoords.x;
				float lambda3 = barycentricCoords.y;

				float z_reciprocal = lambda1 * (1 / vertices[0].z) + lambda2 * (1 / vertices[1].z) + lambda3 * (1 / vertices[2].z);

				glm::vec3 u = worldVertices[0] - worldVertices[1];
				glm::vec3 v = worldVertices[2] - worldVertices[1];
				glm::vec3 face_normal = glm::normalize(-glm::cross(u, v));

				glm::vec3 normal = lambda1 * normals[0] + lambda2 * normals[1] + lambda3 * normals[2];
				glm::vec3 point = lambda1 * worldVertices[0] + lambda2 * worldVertices[1] + lambda3 * worldVertices[2];

				glm::vec3 illumination_color(0,0,0);
				for (int i = 0; i < lights.size(); i++)
				{
					illumination_color += lights[i]->CalculateIllumination(point, normal);
				}

				float r = std::min(color.r * illumination_color.r, 1.0f);
				float g = std::min(color.g * illumination_color.g, 1.0f);
				float b = std::min(color.b * illumination_color.b, 1.0f);

				glm::vec3 final_color = glm::vec3(r, g, b);

				putPixel(x, y, final_color, 1 / z_reciprocal);
			}
		}
	}
}

void Renderer::RenderModel(const Model& model, const Camera& camera, std::vector<std::shared_ptr<Light>> lights)
{
	const glm::mat4x4& world = model.GetWorldTransformation();
	const glm::mat4x4& view = camera.GetCameraTransformation();
	const glm::mat4x4& projection = camera.GetProjectionTransformation();

	int verticesPerElement;
	bool circular;
	switch (model.GetGeometryType())
	{
	case GeometryType::LINES:
		verticesPerElement = 2;
		circular = false;
		break;

	case GeometryType::TRIANGLES:
		verticesPerElement = 3;
		circular = true;
		break;
	}

	int currentVertexIndex = 0;
	int vertexCount = model.GetModelVertexCount();
	while (currentVertexIndex < vertexCount)
	{
		std::vector<glm::vec3> normalElementVertices;
		std::vector<glm::vec3> worldElementVertices;
		std::vector<glm::vec3> transformedElementVertices;
		for (int currentElementVertexIndex = 0; currentElementVertexIndex < verticesPerElement; currentElementVertexIndex++)
		{
			glm::vec4 vertex = glm::vec4(model.GetModelVertex(currentVertexIndex), 1);
			glm::vec4 worldVertex = world * vertex;

			transformedElementVertices.push_back(TransformVertex(worldVertex, glm::mat4x4(1), view, projection));
			normalElementVertices.push_back(model.GetModelNormal(currentVertexIndex));
			worldElementVertices.push_back(worldVertex);

			currentVertexIndex++;
		}
		
		if (model.GetGeometryType() == GeometryType::TRIANGLES)
		{
			DrawLines(transformedElementVertices, glm::vec3(0, 0, 0), circular);
			FillTriangle(transformedElementVertices, worldElementVertices, normalElementVertices, model.GetColor(), lights);
		}
		else
		{
			DrawLines(transformedElementVertices, model.GetColor(), circular);
		}
	}
}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	int cameraCount = scene->GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene->GetModelCount();
		const Camera& camera = scene->GetActiveCamera();
		std::vector<std::shared_ptr<Light>> activeLights = scene->GetActiveLights();
		for(int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<Model> currentModel = scene->GetModel(currentModelIndex);
			RenderModel(*currentModel, camera, activeLights);

			std::shared_ptr<MeshModel> meshModel = std::dynamic_pointer_cast<MeshModel>(currentModel);
			if (meshModel)
			{
				RenderModel(meshModel->GetNormalModel(), camera, activeLights);
			}
		}

		for (int currentCameraIndex = 0; currentCameraIndex < cameraCount; currentCameraIndex++)
		{
			if (currentCameraIndex != scene->GetActiveCameraIndex())
			{
				const Camera& currentCamera = scene->GetCamera(currentCameraIndex);
				RenderModel(currentCamera, camera, activeLights);
			}
		}
	}
}

const int Renderer::GetViewportWidth() const
{
	return viewportWidth;
}

const int Renderer::GetViewportHeight() const
{
	return viewportHeight;
}