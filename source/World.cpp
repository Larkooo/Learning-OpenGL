#include "World.h"
#include "..\include\World.h"

void World::Render()
{
	for (const auto& model : m_Models)
	{
		model->Render();
	}
}
