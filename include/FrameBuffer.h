#pragma once

#include <glad/glad.h>

typedef unsigned int uint32_t;

class FrameBuffer
{
	uint32_t m_Id;

public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void Unbind();
};

