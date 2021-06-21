#version 330 core

layout (location = 0) in vec3 Position;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out mat4 Transform;
out mat4 Model;
out vec4 FragPos;

void main()
{
	Model = uModel;
	Transform = uProjection * uView * uModel;
	FragPos = uModel * vec4(Position, 1.0f);
	gl_Position = Transform * vec4(Position, 1.0f);
}