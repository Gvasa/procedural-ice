#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 lightPosition;

//vec4 positionWorldSpace;

out vec3 normal;
out vec3 vertexPosition_cameraspace;
out vec3 light_direction;

void main(){

	// Output position of the vertex, in clip space : MVP * position   
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0f);

    normal = vertexNormal_modelspace;

    // vertex position in camera space MV * position
    vec3 vertexPosition_cameraspace = vec3(MV * vec4(vertexPosition_modelspace, 1.0f));

    //light position in camera space MV * lightPos
    vec3 lightPosition_cameraspace = vec3(MV * lightPosition);

    //vector from vertex to the lightpos in camera space
    light_direction = normalize(lightPosition_cameraspace - vertexPosition_modelspace);

}

