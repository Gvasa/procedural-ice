#ifndef SCENE_H
#define SCENE_H

#include "Geometry.h"
#include "Controls.h"

#include <GL/gl.h>
#include <glm/gtc/matrix_inverse.hpp>

class Scene {

public:

    Scene();

    ~Scene();

    void initialize();

    void render();

    void addGeometry(Geometry *G) { mGeometries.push_back(G); }

    void updateCameraPosition(double, double);

    void updateCameraZoom(double, double);
    
    void mouseButtonClick(double x, double y) { control->dragStart(x, y); }
    
    void mouseButtonRelease() { control->dragEnd(); }
    
    void resetCamera();

    void setLightColor(glm::vec4 c) { mPointLight.color = c; }

    void setLightPos(glm::vec3 p) { mPointLight.position = p; }

    int getNumGeometries() { return mGeometries.size(); }

    Geometry* getGeometryAt(unsigned int i) { return mGeometries[i]; }

private:

	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mModelMatrix;
	glm::mat4 mMVP;

	Controls *control = nullptr;

	std::vector<glm::mat4> mSceneMatrices;

	std::vector<Geometry *> mGeometries;


	struct LightSource {
		glm::vec3 position;
		glm::vec4 color;
	} mPointLight;

	struct cameraHandler {
        float fov = 45.0f;
        float aspectRatio = 4.0f / 3.0f;
        float zoom = 4.0f;
        glm::quat orientation;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
    } mCamera;
};

#endif // SCENE_H