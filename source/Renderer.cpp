#include "Renderer.h"
#include "..\include\Renderer.h"

void Renderer::Clear()
{
	// Set the color we want to set when clearing
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// clear color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(const VertexArray& vao, const Shader& sProgram, const Camera& camera, std::vector<glm::vec3> cubes, float width, float height)
{
	sProgram.Use();

	sProgram.Set("uFirstTexture", glm::ivec1(0));
	sProgram.Set("uSecondTexture", glm::ivec1(1));


	glm::mat4x4 view = camera.GetView();
	glm::mat4x4 projection(1.0f);


	projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);

	sProgram.Set("uView", view);
	sProgram.Set("uProjection", projection);

	vao.Bind();

	for (const glm::vec3 cube : cubes)
	{
		glm::mat4x4 model(1.0f);
		model = glm::translate(model, cube);
		// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		sProgram.Set("uModel", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	vao.Unbind();
}
