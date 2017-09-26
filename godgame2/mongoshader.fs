#version 330 core
out vec4 FragColor;

in vec2 texPos;
in vec3 realPos;
in vec3 normal;

// texture samplers
//uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform float time;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	vec3 lightOrigin = vec3(0, 1, 1);
	float dist = distance(lightOrigin, realPos);
	float mp = 1;//100 / pow(dist,2);
	float mp2 = dot(normalize(lightOrigin - realPos), normal);
	mp2 = max(0,mp2);
	
	FragColor = mp2  * vec4(1.0, 1.0, 1.0 ,0.0) + vec4(0.2,0.2,0.2, 1.0); //mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}