#pragma once

#include <glad/glad.h>

#include <cstdint>

class IndexBuffer
{
	uint32_t m_Id;
	uint32_t m_Count;
public:
	IndexBuffer(const uint32_t* data, uint32_t count);
	inline IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline uint32_t GetCount() const { return m_Count; }
};

