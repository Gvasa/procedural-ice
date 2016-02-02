#ifndef GEOMETRY_H
#define GEOMETRY_H

#define GLM_FORCE_RADIANS
#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3

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

    void initialize(glm::vec3, glm::vec4);

    void render(std::vector<glm::mat4>, glm::vec3, glm::vec4);

    void loadObject(std::string);

    void translate(glm::vec3);

    void scale(glm::vec3);

    void setColor(glm::vec4 c) { mMaterial.color = c; }

    glm::vec4 getColor() { return mMaterial.color; }

    void initializePhongShader();

    void initializeFrostShader();

    void setPhongShader() { shaderProgram = phongShader; }

    void setFrostShader() { shaderProgram = frostShader; }

private:

    // Shader data
    GLuint vertexArrayID;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint frameBufferID;
    GLuint renderedTexture;
    GLuint depthrenderBufferID;

    GLuint shaderProgram;
    GLuint phongShader;
    GLuint frostShader;
    GLuint shaderTextureProgram;

    // Shader indices for Matrices
    GLint MVPLoc;           // MVP matrix
    GLint MVLoc;            // MV matrix
    GLint MVLightLoc;       // MVLight matrix
    GLint NMLoc;            // NM matrix
    GLint lightPosLoc;      // Light position
    GLint lightColLoc;      // Light Color
    GLint colorLoc;         // Color
    GLint lightAmbLoc;      // Ambient light
    GLint lightDifLoc;      // Diffuse light
    GLint lightSpecLoc;      // Specular light
    GLint specularityLoc;   // Specular constant
    GLint shinynessLoc;     // How much specularity (magnitude)
    GLint timeLoc;			// time running program
    GLint texID;

	std::vector<glm::vec3> mVerts;
	std::vector<glm::vec2> mUvs;
	std::vector<glm::vec3> mNormals;

	glm::vec3 mLightPos;
	glm::vec4 mLightColor;

	ObjectLoader * mObjectLoader = new ObjectLoader();

	struct Material {
		glm::vec4 color;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		float specularity;
		float shinyness;
	} mMaterial;


};

#endif // GEOMETRY_H