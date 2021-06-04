#pragma once

#include <glad/glad.h>

#include <cstdint>

class VertexArray
{
	uint32_t m_Id;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;
};

