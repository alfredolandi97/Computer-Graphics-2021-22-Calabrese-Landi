#version 450

layout(set = 0, binding = 0) uniform globalUniformBufferObject {
	mat4 view;
	mat4 proj;
	vec3 lightPos[8];
	vec3 lightColor;
	vec2 coneInOutDecayExp;
} gubo;

layout(set=1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

vec3 point_light_dir(vec3 lightPos, vec3 pos) {
	return normalize(lightPos-pos);
}

vec3 point_light_color(vec3 lightPos, vec3 pos) {
	// Point light color
	float base = gubo.coneInOutDecayExp.x/length(lightPos-pos);
	float decay = pow(base, gubo.coneInOutDecayExp.y);
	return decay*gubo.lightColor;
}

vec3 Lambert_Diffuse_BRDF(vec3 L, vec3 N, vec3 C) {
	return C*max(dot(N,L), 0);
}

void main() {
	const vec3  diffColor = texture(texSampler, fragTexCoord).rgb;
	const vec3  specColor = vec3(1.0f, 1.0f, 1.0f);
	const float specPower = 150.0f;
	const vec3  L = vec3(-0.4830f, 0.8365f, -0.2588f);
	
	vec3 N = normalize(fragNorm);
	vec3 R = -reflect(L, N);
	vec3 V = normalize(fragPos);

	//Point light
	vec3 tempOutColor = vec3(0.0f,0.0f,0.0f);
	for(int i=0;i<(gubo.lightPos).length();i++){
		tempOutColor+=diffColor *Lambert_Diffuse_BRDF(point_light_dir(gubo.lightPos[i], fragPos), fragNorm, point_light_color(gubo.lightPos[i], fragPos))* 1.5f;
	}
	// Lambert diffuse
	//vec3 diffuse  = diffColor * max(dot(N,L), 0.0f) * 1.5f;
	// Phong specular
	//vec3 specular = specColor * pow(max(dot(R,V), 0.0f), specPower);
	// Hemispheric ambient
	//vec3 ambient  = vec3(0.3f,0.3f, 0.3f) * diffColor;
	
	//outColor = vec4(clamp(ambient + diffuse + specular, vec3(0.0f), vec3(1.0f)), 1.0f);
	outColor = vec4(tempOutColor.xyz, 1.0f);
}