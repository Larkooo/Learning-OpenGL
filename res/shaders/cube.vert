#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TextureCoords;

uniform mat4x4 uModel;
uniform mat4x4 uView;
uniform mat4x4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = aNormal;
    TextureCoords = aTextureCoords;
}