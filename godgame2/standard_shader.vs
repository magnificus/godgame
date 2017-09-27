#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;

out vec2 texPos;
out vec3 realPos;
out vec3 normal;
out vec3 lightOrigin;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
	lightOrigin = vec3(10,10,10);//(projection * view * vec4(1,0,1,1)).xyz;
	texPos = vec2(aPos.x, aPos.y);
	realPos = aPos;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	normal = inNormal;
}