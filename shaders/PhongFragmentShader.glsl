#version 330 core

// basic shading fragment shader, Heavily influenced by opengl tutorial shading

uniform mat4 MVP;
uniform mat4 MV;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightPower;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 cameraPosition;
uniform float alpha;

in vec3 positionWorldspace;
in vec3 normalCameraspace;
in vec3 eyeDirectionCameraspace;
in vec3 lightDirectionCameraspace;
	
out vec4 fragColor;

void main() {

	//the distance from frag to light source
	float lightDistance = length(lightPosition - positionWorldspace );

	//normalized normal of fragment in camera space;
	vec3 normal = normalize( normalCameraspace );

	//direction of the light from frag to light
	vec3 lightDirection = normalize ( lightDirectionCameraspace );

	//cosine of the angle between the normal and the light direction
	// clamped above 0
	// - light is at the vertical of the triangle -> 1
	// - light is perpendicular to the trinalge -> 0
	// - light is behind the triangle -> 0 
	float cosTheta = clamp( dot( normal, lightDirection ), 0, 1);

	// from fragment to camera(eye)
	vec3 eyeDirection = normalize(eyeDirectionCameraspace);

	//direction which the triangle reflects the light \/
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	//cosine of the angle between eye direction and the reflect direction
	//  - looking into the reflection -> 1
	// - looking elsewhere < 1
	float cosAlpha = clamp( dot(eyeDirection, reflectionDirection), 0, 1);

	/*fragColor = 
			//ambient color simulates light form other sources
			ambientColor +
			//diffuse color, which is the color of the object
			diffuseColor * lightColor * lightPower * cosTheta / (lightDistance * lightDistance) +
			// specular : reflective highlight
			specularColor * lightColor * lightPower * pow(cosAlpha, 5) / (lightDistance * lightDistance);
*/	

	/*if(cosTheta < 0.01f)
		fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else
		fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);*/
	fragColor.rgb = ambientColor + 
					diffuseColor * lightColor * lightPower * cosTheta / ( lightDistance * lightDistance) +
					specularColor * lightColor * lightPower * pow(cosAlpha,10) * cosAlpha / (lightDistance * lightDistance);
			 	//diffuseColor * lightColor  * cosTheta / lightDistance;//vec4(1.0f, 1.0f, 1.0f, 1.0f);

	fragColor.a = alpha;
}