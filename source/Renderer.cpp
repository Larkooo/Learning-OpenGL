#include "Renderer.h"
#include "..\include\Renderer.h"

void Renderer::Clear()
{
	// Set the color we want to set when clearing
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// clear color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render()
{
	m_World->Render();
}
