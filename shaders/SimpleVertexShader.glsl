#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;
out vec2 UV

void main(){

    vec4 pos;
    pos.xyz = vertexPosition_modelspace;
    pos.w = 1.0;

    gl_Position = MVP * pos;
}

