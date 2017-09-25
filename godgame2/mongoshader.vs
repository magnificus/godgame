#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoord;

out vec2 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
	pos = vec2(aPos.x, aPos.y);
	gl_Position = projection * view * model * vec4(aPos, 1.0f);// + vec4(pos, 0.0, 0.0)*sin(time);

	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}