#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D uFirstTexture;
uniform sampler2D uSecondTexture;

uniform vec3 uColor;
uniform vec3 uLightColor;

void main()
{
	FragColor = mix(texture(uFirstTexture, TexCoord), texture(uSecondTexture, TexCoord), 0.5f) * vec4(uLightColor * uColor, 1.0f);
}