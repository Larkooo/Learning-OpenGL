#pragma once

typedef unsigned int uint32_t;

class Material
{
	// diffuse texture id
	uint32_t diffuse;
	// specular texture id
	uint32_t specular;
	float shininess;

public:
	inline Material(uint32_t diffuseId, uint32_t specularId, float shininess)
	{
		diffuse = diffuseId;
		specular = specularId;
		this->shininess = shininess;
	}
	inline Material() {};

	inline uint32_t GetDiffuse() const { return diffuse; }
	inline uint32_t GetSpecular() const { return specular; }
	inline float GetShininess() const { return shininess; }
	
	inline uint32_t SetDiffuse(uint32_t diffuseId) { diffuse = diffuseId; }
	inline uint32_t SetSpecular(uint32_t specularId) { specular = specularId; }
	inline float SetShininess(float shininess) { this->shininess = shininess; }
};