#define _USE_MATH_DEFINES
#include <cmath>

#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "ImguiMenus.h"
#include "GridModel.h"
#include "AmbientLight.h"
#include "PointLight.h"

double aspectRatio;
double zoomFactor = 1;
bool zoomChanged = false;

static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, std::shared_ptr<Scene> scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	renderer.SetViewport(0, 0, frameBufferWidth, frameBufferHeight);

	glm::vec4 eye = glm::vec4(0, 0, 5, 1);
	glm::vec4 at = glm::vec4(0, 0, 0, 1);
	glm::vec4 up = glm::vec4(0, 1, 0, 1);
	aspectRatio = float(frameBufferWidth) / float(frameBufferHeight);
	Camera camera = Camera(eye, at, up, std::make_unique<PrespectiveProjection>(M_PI / 4, aspectRatio, 1, 10));

	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	scene->AddCamera(camera);
	scene->AddModel(std::make_shared<GridModel>(12, 12, 0.2, 0.2));

	scene->AddLight(std::make_shared<AmbientLight>());
	scene->AddLight(std::make_shared<PointLight>(scene));

	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(aspectRatio, io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, std::shared_ptr<Scene> scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		renderer.SetViewport(0, 0, frameBufferWidth, frameBufferHeight);
		aspectRatio = float(frameBufferWidth) / float(frameBufferHeight);
		scene->GetActiveCamera().SetProjectionAspectRatio(aspectRatio);
	}

	if (!io.WantCaptureKeyboard)
	{
		if (io.KeysDown[49]) // 1
		{
			scene->GetActiveModel()->SelfRotateX(M_PI / 20);
		}

		if (io.KeysDown[50]) // 2
		{
			scene->GetActiveModel()->SelfRotateX(-M_PI / 20);
		}

		if (io.KeysDown[51]) // 3
		{
			scene->GetActiveModel()->SelfRotateY(M_PI / 20);
		}

		if (io.KeysDown[52]) // 4
		{
			scene->GetActiveModel()->SelfRotateY(-M_PI / 20);
		}

		if (io.KeysDown[53]) // 5
		{
			scene->GetActiveModel()->SelfRotateZ(M_PI / 20);
		}

		if (io.KeysDown[54]) // 6
		{
			scene->GetActiveModel()->SelfRotateZ(-M_PI / 20);
		}

		if (io.KeysDown[45]) // -
		{
			scene->GetActiveModel()->Scale(1 / 1.1);
		}

		if (io.KeysDown[61]) // +
		{
			scene->GetActiveModel()->Scale(1.1);
		}

		if (io.KeysDown[65]) // a
		{
			scene->GetActiveModel()->Translate(glm::vec3(-0.2, 0, 0));
		}

		if (io.KeysDown[68]) // d
		{
			scene->GetActiveModel()->Translate(glm::vec3(0.2, 0, 0));
		}

		if (io.KeysDown[83]) // s
		{
			scene->GetActiveModel()->Translate(glm::vec3(0, 0, 0.2));
		}

		if (io.KeysDown[87]) // w
		{
			scene->GetActiveModel()->Translate(glm::vec3(0, 0, -0.2));
		}
	}

	if (!io.WantCaptureMouse)
	{
		if (io.MouseDown[0])
		{
			ImVec2 mouseDelta = io.MouseDelta;
			scene->GetActiveCamera().SphericalRotate(glm::vec2(mouseDelta.x / 60, mouseDelta.y / 60));
		}

		if (zoomChanged)
		{
			scene->GetActiveCamera().Zoom(zoomFactor);
			zoomChanged = false;
		}
	}

	renderer.ClearColorBuffer(GetClearColor());
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}
