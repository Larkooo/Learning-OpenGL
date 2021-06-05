#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D uFirstTexture;
uniform sampler2D uSecondTexture;

uniform vec3 uViewPos;
uniform vec3 uColor;
uniform vec3 uAmbientColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;

float specularIntensity = 0.5f;

void main()
{
	// diffuse light
    vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(uLightPos - FragPos);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * uLightColor;

	// specular light
	vec3 viewDirection = normalize(uViewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = spec * specularIntensity * uLightColor;

	FragColor = vec4((uAmbientColor + diffuse + specular) * uColor, 1.0f);
}