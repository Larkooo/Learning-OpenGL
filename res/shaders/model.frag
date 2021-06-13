#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

struct Material 
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material uMaterial;


void main()
{
    FragColor = texture(uMaterial.texture_diffuse1, TexCoords);
}