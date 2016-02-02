#version 330 core

uniform mat4 NM;
uniform vec4 color;
uniform vec4 lightAmb;
uniform vec4 lightDif;
uniform vec4 lightSpec;
uniform vec4 lightColor;
uniform float specularity;
uniform float shinyness;

in vec3 normal;
in vec3 v;
in vec3 light_direction;

out vec4 frag_color;

vec4 calcShading( vec3 N, vec3 L) {

	//ambient
	vec4 ambientContrib = lightAmb * lightColor;

	//diffuse
	vec4 diffuseContrib = lightDif * max(dot(N, L), 0.0);
	diffuseContrib = clamp(diffuseContrib, 0.0, 1.0);

	return ambientContrib + diffuseContrib;
}

vec4 calcSpecularShading( vec3 N, vec3 L ) {

	//specular
	vec3 v_dir = normalize(-v);

	vec3 R = normalize(reflect(-L, normalize(N)));

	const float specExp = 10.0;
	float spec = dot(R, v_dir);
	spec = (spec > 0.0) ? (1.0 * pow(spec, specularity)) : 0.0;

	vec4 specularContrib = lightSpec * spec;
	specularContrib = clamp(specularContrib, 0.0, 1.0);

	return specularContrib * lightColor;

}

void main() {
	
	v;
	light_direction;

	frag_color = color;

	frag_color.rgb *= calcShading(normalize(vec3(NM * vec4(normal, 1.0))), light_direction).rgb;

	frag_color.rgb += calcSpecularShading(normalize(vec3(NM * vec4(normal, 1.0))), light_direction).rgb * shinyness;

	//frag_color.rgb *= snoise(vec2(color.r, color.b));

}