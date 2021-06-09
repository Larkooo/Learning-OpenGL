#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec2 TextureCoords;
in vec3 FragPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirectionLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// max hardcoded light
const int NBR_PL = 32;

uniform Material uMaterial;
uniform PointLight uPointLights[NBR_PL];
uniform DirectionLight uDirLight;
uniform int uPointLightsCount;

uniform vec3 uViewPos;

vec3 CalculateDiffuse(vec3 direction, vec3 norm, vec3 diffuse);
vec3 CalculateSpecular(vec3 direction, vec3 norm, vec3 viewDirection, vec3 specular);
vec3 CalculateDirLight(DirectionLight light, vec3 norm, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDirection);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDirection = normalize(uViewPos - FragPos);
	
	vec3 result = CalculateDirLight(uDirLight, norm, viewDirection);
	for (int i = 0; i < uPointLightsCount; i++)
		result += CalculatePointLight(uPointLights[i], norm, viewDirection);

	FragColor = vec4(result, 1.0f);
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDirection)
{
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TextureCoords));
	vec3 diffuse = CalculateDiffuse(lightDir, norm, light.diffuse);
	vec3 specular = CalculateSpecular(lightDir, norm, viewDirection, light.specular);

	float distance = length(light.position - FragPos);

	// apply attenuation
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalculateDirLight(DirectionLight light, vec3 norm, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.direction);
	vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TextureCoords));
	vec3 diffuse = CalculateDiffuse(lightDir, norm, light.diffuse);
	vec3 specular = CalculateSpecular(lightDir, norm, viewDirection, light.specular);
	return (ambient + diffuse + specular);
}

vec3 CalculateDiffuse(vec3 direction, vec3 norm, vec3 diffuse)
{
	// diffuse formula
	float diff = max(dot(norm, direction), 0.0);
	// apply it to our texture and then apply the light
	return (diffuse * (diff * vec3(texture(uMaterial.diffuse, TextureCoords))));
}

vec3 CalculateSpecular(vec3 direction, vec3 norm, vec3 viewDirection, vec3 specular)
{
	vec3 reflectDirection = reflect(-direction, norm);
	// specular formula
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterial.shininess);
	// apply it to our texture and then apply the light
	return (specular * (spec * vec3(texture(uMaterial.specular, TextureCoords))));
}