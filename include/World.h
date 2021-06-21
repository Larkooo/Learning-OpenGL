#pragma once

#include "Object.h"

class World
{
	std::vector<std::unique_ptr<Object>> m_Models;

public:
	//inline World(std::vector<std::unique_ptr<Object>> models) : m_Models(models) {}
	inline World() {}

	void Render();
};

