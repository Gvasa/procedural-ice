#include "../include/Scene.h"

Scene::Scene() {

}

Scene::~Scene() {
 	
 	 for(std::vector<Geometry *>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
 	 	(*it)->~Geometry();

}

void Scene::initialize() {

	std::cout << "\nInitializing Scene\n" << std::endl;

	glClearColor(0.2f, 0.2f, 0.2f, 0.0);

	//set up camera
	mCamera = new Camera(glm::vec3(0.0, 0.0, 2.0f));

	//Lightsource parameters    
    mPointLight.position = glm::vec3(0.0f, 4.0f, 4.0f);
    mPointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    mPointLight.power = 50.0f;

    for(std::vector<Geometry *>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
    	(*it)->initialize(mPointLight.position, mPointLight.color, mCamera->getPosition());

    std::cout << "\nScene initialized!\n" << std::endl;
}

void Scene::render() {

	mCamera->update();

	mSceneMatrices.resize(6);

	//mvp matrix
	mSceneMatrices[I_MVP] = mCamera->getProjectionMatrix() * mCamera->getViewMatrix() * mCamera->getModelMatrix();

	//Modelview matrix
	mSceneMatrices[I_MV] = mCamera->getViewMatrix() * mCamera->getModelMatrix();

	// Model Matrix
	mSceneMatrices[I_M] = mCamera->getModelMatrix();

	// ViewMatrix
	mSceneMatrices[I_V] = mCamera->getViewMatrix();

	// modelview matrix for our light
	mSceneMatrices[I_MV_LIGHT] = mCamera->getViewMatrix() * mCamera->getModelMatrix();

	// normal matrix, normals in shading
	mSceneMatrices[I_NM] = glm::inverseTranspose(mCamera->getViewMatrix() * mCamera->getModelMatrix());
 	
	for(std::vector<Geometry *>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
		(*it)->render(mSceneMatrices, mPointLight.position, mPointLight.color, mPointLight.power);

}

void Scene::updateCameraPosition(double x, double y) {
    if(! mCamera->dragged())
        return;

    mCamera->rotate(mCamera->getOrientation(), x, y);
    mCamera->dragUpdate(x, y);
}

void Scene::updateCameraZoom(double x, double y) {

    if((3.0 + (mCamera->getZoom() - y / 5.0f)) > 0.1f)
        mCamera->setZoom(mCamera->getZoom() - y / 5.0f);
}

void Scene::resetCamera() {
    mCamera->reset();
}