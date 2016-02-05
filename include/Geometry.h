#ifndef GEOMETRY_H
#define GEOMETRY_H

#define GLM_FORCE_RADIANS
#define I_MVP 0
#define I_MV 1
#define I_M 2
#define I_V 3
#define I_MV_LIGHT 4
#define I_NM 5

#define PHONGSHADER 0
#define FROSTSHADER 1

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>


#include "shader.hpp"
#include "../include/math/Vector3.h"
#include "../include/math/Vector4.h"
#include "../include/ObjectLoader.h"
#include <GLFW/glfw3.h>

class Geometry {

public:

    Geometry();

    ~Geometry();

    void initialize(glm::vec3, glm::vec3, glm::vec3);

    void render(std::vector<glm::mat4>, glm::vec3, glm::vec3, float);

    void renderPhong(std::vector<glm::mat4>, glm::vec3, glm::vec3, float);

    void loadObject(std::string);

    void translate(glm::vec3);

    void scale(glm::vec3);

    void setColor(glm::vec4 c) { mMaterial.diffuseColor = glm::vec3(c[0], c[1], c[2]); mMaterial.alpha = c[3]; }

    glm::vec3 getColor() { return mMaterial.diffuseColor; }

    float getAlpha() { return mMaterial.alpha; }

    void setPhongActive() { shaderProgram = phongShader; }

    void setFrostActive() { shaderProgram = frostShader; }

    bool phongActive() { return phongShader == shaderProgram; }

private:

    // Shader data
    GLuint vertexArrayID;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint frameBufferID;
    GLuint depthrenderBuffer;

    GLuint frostTextureBuffer;
    GLuint frostTexture;

    GLuint shaderProgram;
    GLuint phongShader;
    GLuint frostShader;
    GLuint shaderTextureProgram;

    // Shader indices for Matrices
    GLint MVPLoc;           // MVP matrix
    GLint MVLoc;            // MV matrix
    GLint MLoc;				// Model Matrix
    GLint VLoc;				// View Matrix
    GLint MVLightLoc;       // MVLight matrix
    GLint NMLoc;            // NM matrix
    GLint cameraPosLoc;		// camera position
    GLint lightPosLoc;      // Light position
    GLint lightColLoc;      // Light Color
    GLint lightPowerLoc;
    GLint colorLoc;         // Color
    GLint ambienceLoc;      // Ambient light
    GLint diffuseLoc;      // Diffuse light
    GLint specularLoc;      // Specular light
    GLint alphaLoc;			// alpha loc
    GLint specularityLoc;   // Specular constant
    GLint shinynessLoc;     // How much specularity (magnitude)
    GLint timeLoc;			// time running program

    GLuint quad_programID;
    GLuint quad_vertexbuffer;
    GLint texID;
    GLint timeID;

    

	std::vector<glm::vec3> mVerts;
	std::vector<glm::vec2> mUvs;
	std::vector<glm::vec3> mNormals;

	glm::vec3 mLightPos;
	glm::vec3 mLightColor;

	ObjectLoader * mObjectLoader = new ObjectLoader();

	struct Material {
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float alpha;
		float specularity;
		float shinyness;
	} mMaterial;


};

#endif // GEOMETRY_H