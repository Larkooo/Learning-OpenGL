#pragma once

#include <glad/glad.h>

#include "stb_image.h"

#include <cstdint>
#include <iostream>

class Texture
{
	uint32_t m_Id;
	int m_Width, m_Height;
public:
	Texture(const char* filePath);
	~Texture();

	void Bind() const;
	void Unbind() const;
};

