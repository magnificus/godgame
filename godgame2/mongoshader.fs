#version 330 core
out vec4 FragColor;

in vec2 pos;
//in vec3 pos;

// texture samplers
//uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform float time;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(pos, 1.0,1.0); //mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}