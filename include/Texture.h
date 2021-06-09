#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

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

	inline uint32_t GetId() { return m_Id; }
	inline glm::ivec2 GetSize() { return { m_Width, m_Height }; }
	
	void Bind() const;
	void Unbind() const;

	inline void operator=(const Texture& newTexture)
	{
		glDeleteTextures(1, &m_Id);
		m_Id = newTexture.m_Id;
		m_Width = newTexture.m_Width, m_Height = newTexture.m_Height;
	}
};

