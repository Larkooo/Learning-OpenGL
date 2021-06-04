#include "Texture.h"
#include "..\include\Texture.h"

Texture::Texture(const char* filePath)
{
	//std::cout << filePath << std::endl;
	stbi_set_flip_vertically_on_load(true);

	int nrChannels;
	unsigned char* data = stbi_load(filePath, &m_Width, &m_Height, &nrChannels, 0);

	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Id);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, m_Id);
}
