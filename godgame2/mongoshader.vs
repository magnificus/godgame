#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;

out vec2 texPos;
out vec3 realPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
	texPos = vec2(aPos.x, aPos.y);
	realPos = aPos;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);// + vec4(pos, 0.0, 0.0)*sin(time);
	normal = inNormal;
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}