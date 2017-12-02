#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;
uniform bool shadows;
uniform float timeExisted;
uniform mat4 model;
uniform mat4 mvp;


uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);  

float rand( vec2 p )
{
    vec2 K1 = vec2(
        23.14069263277926, // e^pi (Gelfond's constant)
         2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
    );
    return fract( cos( dot(p,K1) ) * 12345.6789 );
}

float ShadowCalculation(vec3 fragPos, vec3 normal)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
	float currentDepth = length(fragToLight)/far_plane;

	float shadow = 0.0;
	float bias   = 0.00015;
	int samples  = 20;
	float viewDistance = length(viewPos - fragPos);
	//float diskRadius = 0.02;
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 50.0;  

	for(int i = 0; i < 0; ++i)
	{
		float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;   // Undo mapping [0;1]
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);  

    shadow = currentDepth -  bias > closestDepth? 1.0 : 0.0;
	return shadow;
}  

void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.2 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
	diff = 1 / (1+pow(2.71828,-diff*3)) - 0.5;

		
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow

	float shadow = 0.0;      
	if (shadows)
		shadow = ShadowCalculation(fs_in.FragPos, normal);
    //vec3 lighting = (ambient + (1.0 - shadow) * (diff + specular)) * color;
	float len = pow(length(fs_in.FragPos - lightPos),1.0/2.0)/10;
	vec3 lighting = (ambient + max(0.0, 1-len)*0.3 + ((1.0-shadow)*(diff))*0.5) * color;

	float randN =  rand((fs_in.FragPos.xz + fs_in.FragPos.xy + fs_in.FragPos.yz)*timeExisted);
	float randN2 =  rand((fs_in.FragPos.xz + fs_in.FragPos.xy + fs_in.FragPos.yz)*timeExisted*2);

	float trans = timeExisted/2 - randN;
	//trans = 0.4;
	trans = min(trans,1);
	lighting += pow((1-trans),100)*randN2;
    FragColor = vec4(lighting, trans);

}  