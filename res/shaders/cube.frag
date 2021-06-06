#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D uFirstTexture;
uniform sampler2D uSecondTexture;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material uMaterial;
uniform Light uLight;

uniform vec3 uViewPos;
uniform vec3 uColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;

vec3 norm = normalize(Normal);
vec3 uLightDirection = normalize(uLight.position - FragPos);

vec3 CalculateDiffuse()
{
	float diff = max(dot(norm, uLightDirection), 0.0);
	vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);
	return diffuse;
}

vec3 CalculateSpecular()
{
	vec3 viewDirection = normalize(uViewPos - FragPos);
	vec3 reflectDirection = reflect(-uLightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * (spec * uMaterial.specular);
	return specular;
}

void main()
{
	// ambient
	vec3 ambient = uLight.ambient * uMaterial.ambient;

	FragColor = vec4((ambient + CalculateDiffuse() + CalculateSpecular()) * uColor, 1.0f);
}