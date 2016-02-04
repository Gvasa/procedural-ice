#version 330 core

//Heavily influeced by openGL tutorials for basic shading

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPositionModelspace;
layout(location = 1) in vec3 vertexNormalModelspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPosition;

out vec3 positionWorldspace;
out vec3 normalCameraspace;
out vec3 eyeDirectionCameraspace;
out vec3 lightDirectionCameraspace;


void main() {

	//output pos of the vertex in clip space: MVP * pos
	gl_Position = MVP * vec4(vertexPositionModelspace, 1.0);

	// position of vertex in worldspace: M * pos
	positionWorldspace = vec3(M * vec4(vertexPositionModelspace, 1.0));

	// vector, eyeDirectionCameraspace, that goes from the vertex to the camera in camera space.
	vec3 vertexPositionCameraspace = vec3(V * M * vec4(vertexPositionModelspace, 1.0));
	eyeDirectionCameraspace = vec3(0.0, 0.0, 0.0) - vertexPositionCameraspace;

	// vector from the vertex to the light in camera space, M = identity
	vec3 lightPositionCameraspace = vec3( V * vec4(lightPosition, 1.0));
	lightDirectionCameraspace = lightPositionCameraspace + eyeDirectionCameraspace;

	// Normal of the vertex in camera space
	normalCameraspace = vec3(transpose(inverse(V * M)) * vec4(vertexNormalModelspace, 1.0));

}