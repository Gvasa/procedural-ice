#ifndef SCENE_H
#define SCENE_H

#include "Geometry.h"
#include "Camera.h"

#include <GL/gl.h>


class Scene {

public:

    Scene();

    ~Scene();

    void initialize();

    void render();

    void addGeometry(Geometry *G) { mGeometries.push_back(G); }

    void updateCameraPosition(double, double);

    void updateCameraZoom(double, double);
    
    void mouseButtonClick(double x, double y) { mCamera->dragStart(x, y); }
    
    void mouseButtonRelease() { mCamera->dragEnd(); }
    
    void resetCamera();

    void setLightColor(glm::vec3 c) { mPointLight.color = c; }

    void setLightPos(glm::vec3 p) { mPointLight.position = p; }

    int getNumGeometries() { return mGeometries.size(); }

    Geometry* getGeometryAt(unsigned int i) { return mGeometries[i]; }

private:

	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mModelMatrix;
	glm::mat4 mMVP;

	Camera * mCamera = nullptr;

	std::vector<glm::mat4> mSceneMatrices;

	std::vector<Geometry *> mGeometries;

	struct LightSource {
		glm::vec3 position;
		glm::vec3 color;
		float power;
	} mPointLight;

};

#endif // SCENE_H