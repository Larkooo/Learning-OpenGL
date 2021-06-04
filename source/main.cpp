#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Renderer.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

int width = 1280, height = 720;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec2 lastMouse(width / 2.0f, height / 2.0f);
bool firstMouse = true;

bool showCursor = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// Initialize glfw
	glfwInit();

	// Set "assertions", if those checks are false, opengl will not render anything
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Load our os specific functions thanks to glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set initially our viewport to 800 800, our base size
	glViewport(0, 0, width, height);

	// register the window resize and mouse callback 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	VertexArray vao;
	vao.Bind();

	float vertices[] = {
		// position           texture coords
		// back face (CCW winding)
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		// front face (CCW winding)
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// left face (CCW)
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		// right face (CCW)
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// bottom face (CCW)      
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		// top face (CCW)
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	};

	VertexBuffer vbo(vertices, sizeof(vertices));
	// dont need to bind, already bound 

	/* uint32_t indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	IndexBuffer ebo(indices, 6);
	// dont need to bind already boud */

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	
	VertexArray lightVao;
	lightVao.Bind();
	vbo.Bind();

	// only use position, dont need texture
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	lightVao.Unbind();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Textures
	glActiveTexture(GL_TEXTURE0);
	Texture fTexture("./res/textures/container.jpg");
	glActiveTexture(GL_TEXTURE1);
	Texture sTexture("./res/textures/container.jpg");

	// Shaders
	Shader cubeShader("./res/shaders/cube.vert", "./res/shaders/cube.frag");
	Shader lightShader("./res/shaders/light.vert", "./res/shaders/light.frag");

	struct Cube 
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	struct Light : Cube
	{
		float strength;
	};

	std::vector<Cube> cubes = {
		{ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f} },
		//{0.0f, 0.0f, -10.0f}
	};

	Light ambientLight;
	ambientLight.position = { 0.0f, 10.0f, 0.0f };
	ambientLight.color = { 1.0f, 1.0f, 1.0f };
	ambientLight.strength = 0.1f;
	
	int selectedObject = 0;
	Renderer renderer;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// process inputs
		processInput(window);

		renderer.Clear();
		//renderer.Render(vao, sProgram, camera, cubes, width, height);

		// wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4x4 view = camera.GetView();
		glm::mat4x4 projection(1.0f);

		projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);

		// Lights
		lightShader.Use();
		
		lightShader.Set("uView", view);
		lightShader.Set("uProjection", projection);
		
		lightVao.Bind();
		
		glm::mat4x4 model(1.0f);
		
		model = glm::translate(model, ambientLight.position);
		lightShader.Set("uModel", model);
		lightShader.Set("uColor", ambientLight.color * ambientLight.strength);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cubes
		cubeShader.Use();

		cubeShader.Set("uFirstTexture", glm::ivec1(0));
		cubeShader.Set("uSecondTexture", glm::ivec1(1));

		cubeShader.Set("uView", view);
		cubeShader.Set("uProjection", projection);

		vao.Bind();

		for (const Cube cube : cubes)
		{
			glm::mat4x4 model(1.0f);
			model = glm::translate(model, cube.position);
			// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			cubeShader.Set("uModel", model);
			cubeShader.Set("uColor", cube.color);
			cubeShader.Set("uLightColor", ambientLight.color * ambientLight.strength);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// imgui render
		ImGui::Begin("Camera");

		ImGui::Text("Speed");
		ImGui::InputFloat("", &Camera::Movement::Speed);
		ImGui::SliderFloat("", &Camera::Movement::Speed, 1.0f, 50.0f);

		ImGui::Spacing();

		ImGui::Text("Position");
		ImGui::InputFloat("X", &camera.position.x);
		ImGui::InputFloat("Y", &camera.position.y);
		ImGui::InputFloat("Z", &camera.position.z);
		ImGui::Text("Rotation");
		ImGui::InputFloat("X", &camera.rotation.x);
		ImGui::InputFloat("Y", &camera.rotation.y);
		ImGui::InputFloat("Z", &camera.rotation.z);

		ImGui::Spacing();

		ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 90.0f, std::to_string(camera.fov).c_str(), 1.0f);
		ImGui::End();

		ImGui::Begin("Ambient light");
		
		ImGui::SliderFloat("Strength", &ambientLight.strength, 0.0f, 1.0f);

		ImGui::Spacing();
		
		float* lightColors[3] = { &ambientLight.color.x, &ambientLight.color.y, &ambientLight.color.z };
		ImGui::ColorPicker3("Color", *lightColors);
		
		ImGui::Text("Position");
		float* lightPosition[3] = { &ambientLight.position.x, &ambientLight.position.y, &ambientLight.position.z };
		ImGui::InputFloat3("Input", *lightPosition);
		
		ImGui::End();

		ImGui::Begin("Objects");
		for (uint32_t i = 0; i < cubes.size(); i++)
			if (ImGui::Button(std::string(std::to_string(i) + "# object").c_str()))
				selectedObject = i;
		ImGui::End();

		ImGui::Begin("Object");
		ImGui::Text("Position");
		float* position[3] = { &cubes.at(selectedObject).position.x, &cubes.at(selectedObject).position.y, &cubes.at(selectedObject).position.z };
		ImGui::InputFloat3("Input", *position);
		ImGui::End();


		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap the front and back buffers so we can show the latest rendered image
		glfwSwapBuffers(window);
		// get the events (keyboard mouse...)
		glfwPollEvents();
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (showCursor) return;
	glm::vec2 pos = glm::vec2(xpos, ypos);
	glm::vec2 offset;

	if (firstMouse)
	{
		lastMouse = pos;
		firstMouse = false;
	}

	offset.x = pos.x - lastMouse.x;
	offset.y = lastMouse.y - pos.y;

	lastMouse = pos;

	camera.UpdateRotation(offset);
}

int multiplier = 1;
float lastUsage = -5.0f;
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && (glfwGetTime() - lastUsage > 5.0f))
	{
		lastUsage = glfwGetTime();
		showCursor = !showCursor;
		glfwSetInputMode(window, GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
		

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);		

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.UpdatePosition(Camera::Movement::Direction::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.UpdatePosition(Camera::Movement::Direction::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.UpdatePosition(Camera::Movement::Direction::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.UpdatePosition(Camera::Movement::Direction::RIGHT, deltaTime);
}

// set the new dimensions to the viewport everytime a user resizes the window
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	width = w, height = h;
	glViewport(0, 0, w, h);
}