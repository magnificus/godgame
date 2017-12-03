#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat3 normalizer;
uniform mat4 mvp;
uniform mat4 model;
uniform float timeExisted;

float rand( vec2 p )
{
    vec2 K1 = vec2(
        23.14069263277926, // e^pi (Gelfond's constant)
         2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
    );
    return fract( cos( dot(p,K1) ) * 12345.6789 );
}



void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vec3 extra = vec3(0,0,0);
	vs_out.Normal =  normalizer * aNormal;

	if (timeExisted < 3.0){
		//float r1 = rand(vs_out.FragPos.xy + vs_out.FragPos.xz + vs_out.FragPos.yz) - 0.5;
		//float r2 = rand(vs_out.FragPos.xy*2 + vs_out.FragPos.xz + vs_out.FragPos.yz) - 0.5;
		//float r3 = rand(vs_out.FragPos.xy + vs_out.FragPos.xz*2 + vs_out.FragPos.yz) - 0.5;

		extra = normalize(aNormal) * pow((3.0-timeExisted),2);
		//extra = vec3(sin(aNormal.x*20), sin(aNormal.y*13), sin(aNormal.z*11))*(3-timeExisted);
	}

    gl_Position = mvp * vec4(aPos + extra, 1.0); //+ vec4(extra, 0.0);
}  