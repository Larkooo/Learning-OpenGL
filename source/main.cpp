#include <glad/glad.h>

#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <Windows.h>
#endif

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
#include "Model.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>

int width = 1280, height = 720;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// imgui
void Style();

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
	io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Style();

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	VertexArray vao;
	vao.Bind();
	VertexBuffer vbo(vertices, sizeof(vertices));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	VertexArray lightVao;
	lightVao.Bind();
	// we use the same vbo because we want to also use a cube for
	// the mesh of the lights. we wont use the normals and texture coordinates tho (for now atleast)
	vbo.Bind();

	// only use position, dont need texture
	glEnableVertexAttribArray(0);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	lightVao.Unbind();

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);

	// Textures
	glActiveTexture(GL_TEXTURE0);
	Texture container("./res/textures/container2.png");
	glActiveTexture(GL_TEXTURE1);
	Texture containerSpecular("./res/textures/container2_specular.png");
	glActiveTexture(GL_TEXTURE2);
	Texture containerEmission("./res/textures/matrix.jpg");

	// Models
	Model backpack("C:/Users/Larko/Desktop/development/cpp_projects/LearnOpenGL/resources/objects/backpack/backpack.obj");

	// Shaders
	Shader cubeShader("./res/shaders/cube.vert", "./res/shaders/cube.frag");
	Shader lightShader("./res/shaders/light.vert", "./res/shaders/light.frag");
	Shader modelShader("./res/shaders/model.vert", "./res/shaders/model.frag");

	struct Material
	{
		// diffuse texture index
		int diffuse;
		// specular texture index
		int specular;
		// emission texture index
		int emission;
		float shininess;
	};

	struct Cube 
	{
		glm::vec3 position;
		Material material;
		bool render;
	};

	struct DirectionLight
	{
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float strength;
		
		bool render;
	};

	struct PointLight
	{
		glm::vec3 position;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;

		float strength;
		
		bool render;
	};

	/* struct Light : Cube
	{
		float strength;
	};*/

	std::vector<Cube> cubes = {
		{ {0.0f, 0.0f, 0.0f}, { 0, 1, 2, 32.0f }, true },
	};

	/* for (int x = 0; x < 15; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			for (int z = 0; z < 15; z++)
			{
				Cube cube;
				cube.position = { x, y, z };
				cube.color = { 1.0f, 1.0f, 1.0f };
				cubes.push_back(cube);
			}
		}
	} */

	std::vector<PointLight> lights = std::vector<PointLight>();

	// DirectionLight dirLight = { {0.5f, 0.2f, 0.4f},  };

	PointLight pointLight;
	pointLight.position = { 3.0f, 3.0f, 0.0f };
	pointLight.ambient = { 0.2f, 0.2f, 0.2f };
	pointLight.diffuse = { 0.5f, 0.5f, 0.5f };
	pointLight.specular = { 1.0f, 1.0f, 1.0f };
	pointLight.constant = 1.0f;
	pointLight.linear = 0.09f;
	pointLight.quadratic = 0.032f;
	pointLight.strength = 1.0f;
	pointLight.render = true;

	PointLight pointLight2;
	pointLight2.position = { 3.0f, 3.0f, 0.0f };
	pointLight2.ambient = { 0.2f, 0.2f, 0.2f };
	pointLight2.diffuse = { 0.5f, 0.5f, 0.5f };
	pointLight2.specular = { 1.0f, 1.0f, 1.0f };
	pointLight2.constant = 1.0f;
	pointLight2.linear = 0.09f;
	pointLight2.quadratic = 0.032f;
	pointLight2.strength = 1.0f;
	pointLight2.render = true;

	lights.push_back(pointLight);
	lights.push_back(pointLight2);

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
		
		//renderer.Render();

		// wireframe mode
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4x4 view = camera.GetView();
		glm::mat4x4 projection(1.0f);

		projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);

		// Lights
		lightShader.Bind();

		lightShader.Set("uView", view);
		lightShader.Set("uProjection", projection);

		lightVao.Bind();

		int lightIndex = 0;
		for (const PointLight& light : lights)
		{
			glm::mat4x4 model(1.0f);
			model = glm::translate(model, light.position);
			lightShader.Bind();
			lightShader.Set("uModel", model);
			lightShader.Set("uColor", (light.ambient * light.diffuse * light.specular) * light.strength);
			
			if (light.render)
				glDrawArrays(GL_TRIANGLES, 0, 36);
			lightIndex++;
		}

		// Cubes
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container.GetId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, containerSpecular.GetId());
		// glBindTexture(GL_TEXTURE_2D, containerEmission.GetId());
		cubeShader.Bind();

		cubeShader.Set("uView", view);
		cubeShader.Set("uProjection", projection);

		cubeShader.Set("uViewPos", camera.position);

		cubeShader.Set("uDirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		cubeShader.Set("uDirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		cubeShader.Set("uDirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		cubeShader.Set("uDirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		vao.Bind();

		for (const Cube& cube : cubes)
		{
			glm::mat4x4 model(1.0f);
			model = glm::translate(model, cube.position);
			// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			cubeShader.Set("uModel", model);
			cubeShader.Set("uMaterial.diffuse", glm::ivec1(cube.material.diffuse));
			cubeShader.Set("uMaterial.specular", glm::ivec1(cube.material.specular));
			cubeShader.Set("uMaterial.emission", glm::ivec1(cube.material.emission));
			cubeShader.Set("uMaterial.shininess", glm::vec1(cube.material.shininess));
			cubeShader.Set("uPointLightsCount", glm::ivec1(lights.size()));
			for (int i = 0; i < lights.size(); i++)
			{
				cubeShader.Set(std::string("uPointLights[" + std::to_string(i) + "]" + ".position"), lights.at(i).position);
				cubeShader.Set(std::string("uPointLights[" + std::to_string(i) + "]" + ".ambient"), lights.at(i).ambient);
				cubeShader.Set(std::string("uPointLights[" + std::to_string(i) + "]" + ".diffuse"), lights.at(i).diffuse);
				cubeShader.Set(std::string("uPointLights[" + std::to_string(i) + "]" + ".specular"), lights.at(i).specular);
				cubeShader.Set(std::string("uPointLights[" + std::to_string(i) + "]" + ".constant"), glm::vec1(lights.at(i).constant));
				cubeShader.Set(std::string("uPointLights[" + std::to_string(i) + "]" + ".linear"), glm::vec1(lights.at(i).linear));
				cubeShader.Set(std::string("uPointLights[" + std::to_string(i) + "]" + ".quadratic"), glm::vec1(lights.at(i).quadratic));
			}
			if (cube.render)
				glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		vao.Unbind();

		modelShader.Bind();

		modelShader.Set("uProjection", projection);
		modelShader.Set("uView", view);
		glm::mat4x4 model(1.0f);
		model = glm::translate(model, glm::vec3(15.0f, 10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		modelShader.Set("uModel", model);
		backpack.Render(modelShader);

		modelShader.Unbind();

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// imgui render
		// 
		// Camera
		{
			ImGui::Begin("Camera");

			ImGui::Text("Speed");
			ImGui::SliderFloat("##speed", &Camera::Movement::Speed, 1.0f, 50.0f);

			ImGui::Spacing();

			ImGui::Text("Position");
			ImGui::InputFloat3("XYZ", &camera.position.x);
			ImGui::Text("Rotation");
			ImGui::InputFloat3("XYZ", &camera.rotation.x);

			ImGui::Spacing();

			ImGui::Text("FOV");
			ImGui::SliderFloat("##fov", &camera.fov, 0.0f, 180.0f);
			ImGui::End();

			// Objects
			ImGui::Begin("Objects");
			for (uint32_t i = 0; i < cubes.size(); i++)
				if (ImGui::Button(std::string(std::to_string(i) + "# object").c_str()))
					selectedObject = i;
			ImGui::End();
		}

		// Object
		{
			ImGui::Begin("Object");

			ImGui::Text("Position");
			ImGui::SliderFloat3("XYZ", &cubes.at(selectedObject).position.x, -100.0f, 100.0f);

			ImGui::Spacing();

			ImGui::Text("Material");
			
			ImGui::Text("Diffuse");
			ImGui::Text("Size : %i x %i", container.GetSize().x, container.GetSize().y);
			ImGui::ImageButton((void*)(uintptr_t)container.GetId(), ImVec2(64, 64));

			ImGui::Text("Specular");
			ImGui::Text("Size : %i x %i", containerSpecular.GetSize().x, containerSpecular.GetSize().y);
			ImGui::ImageButton((void*)(uintptr_t)containerSpecular.GetId(), ImVec2(64, 64));
			/*if (ImGui::ImageButton((void*)(uintptr_t)containerSpecular.GetId(), ImVec2(64, 64)))
			{
				OPENFILENAMEA ofn;       // common dialog box structure
				char szFile[260];       // buffer for file name
				HWND hwnd = NULL;              // owner window
				HANDLE hf;              // file handle

				// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFile = szFile;
				//
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				//
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 

				if (GetOpenFileNameA(&ofn))
				{
					glActiveTexture(GL_TEXTURE1);
					containerSpecular = Texture(ofn.lpstrFile);
				}		
			} TODO*/
			ImGui::SliderFloat("Shininess", &cubes.at(selectedObject).material.shininess, 0.0f, 100.0f);

			ImGui::Checkbox("Render", &cubes.at(selectedObject).render);

			ImGui::End();
		}
		// Light
		{
			ImGui::Begin("Light");

			// ImGui::SliderFloat("Strength", &lights.at(0).strength, 0.0f, 1.0f);

			// ImGui::Spacing();

			ImGui::Text("Position");
			ImGui::SliderFloat3("XYZ", &lights.at(0).position.x, -100.0f, 100.0f);

			ImGui::Spacing();

			ImGui::ColorEdit3("Ambient", &lights.at(0).ambient.x);
			ImGui::ColorEdit3("Diffuse", &lights.at(0).diffuse.x);
			ImGui::ColorEdit3("Specular", &lights.at(0).specular.x);

			ImGui::End();
		}

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
	if (showCursor)
	{
		lastMouse = { xpos, ypos };
		return;
	}

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

const float UI_SWITCH_COOLDOWN = 0.3f;
float lastUsage = -5.0f;
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && (glfwGetTime() - lastUsage > UI_SWITCH_COOLDOWN))
	{
		lastUsage = glfwGetTime();
		showCursor = !showCursor;
		glfwSetInputMode(window, GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		showCursor == false ? 
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse 
			: ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
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

void Style()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	/// 0 = FLAT APPEARENCE
	/// 1 = MORE "3D" LOOK
	int is3D = 0;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding = ImVec2(6, 4);
	style.ItemSpacing = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = is3D;

	style.WindowRounding = 3;
	style.ChildRounding = 3;
	style.FrameRounding = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK 
	style.TabBorderSize = is3D;
	style.TabRounding = 3;

	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
#endif
}