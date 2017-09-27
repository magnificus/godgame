#version 330 core
out vec4 FragColor;

in vec2 texPos;
in vec3 realPos;
in vec3 normal;
in vec3 lightOrigin;

uniform float time;

void main()
{
	float dist = distance(lightOrigin, realPos);
	float mp = max(0, dot(normalize(lightOrigin - realPos), normal));
	
	FragColor = mp * vec4(0.5, 0.5, 0.7 ,0.0) + vec4(0.2,0.2,0.25, 1.0); //mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}