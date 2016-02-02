#include "../include/Scene.h"

Scene::Scene() {
	control = new Controls(300.f);
}

Scene::~Scene() {
 	
 	 for(std::vector<Geometry *>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
 	 	(*it)->~Geometry();

}

void Scene::initialize() {
	
	//Lightsource parameters    
    mPointLight.position = glm::vec3(0.0f, 2.0f, 2.0f);
    mPointLight.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glClearColor(0.2f, 0.2f, 0.2f, 0.0);

    for(std::vector<Geometry *>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
    	(*it)->initialize(mPointLight.position, mPointLight.color);

    mSceneMatrices.resize(4);
}

void Scene::render() {


  	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	mCamera.projectionMatrix = glm::perspective(
		mCamera.fov,				// field of view, 45.0
		mCamera.aspectRatio,			// aspect ratio, 4/3
		0.1f,				// near clipping plane
		100.0f);			// far clipping plane

	mCamera.viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.0f + mCamera.zoom), 	// camera position
		glm::vec3(0.0f, 0.0f, 0.0f), 	// look at center
		glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::mat4_cast(mCamera.orientation);	// up vector

	mModelMatrix = glm::mat4(1.0f);

	//mvp matrix
	mSceneMatrices[I_MVP] = mCamera.projectionMatrix * mCamera.viewMatrix * mModelMatrix;

	//Modelview matrix
	mSceneMatrices[I_MV] = mCamera.viewMatrix * mModelMatrix;

	// modelview matrix for our light
	mSceneMatrices[I_MV_LIGHT] = mCamera.viewMatrix * mModelMatrix;

	// normal matrix, normals in shading
	mSceneMatrices[I_NM] = glm::inverseTranspose(mCamera.viewMatrix * mModelMatrix);
 	


	for(std::vector<Geometry *>::iterator it = mGeometries.begin(); it != mGeometries.end(); ++it)
		(*it)->render(mSceneMatrices, mPointLight.position, mPointLight.color);

	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

}

void Scene::updateCameraPosition(double x, double y) {
    if(! control->dragged())
        return;

    control->rotate(mCamera.orientation, x, y);
    control->dragUpdate(x, y);
}

void Scene::updateCameraZoom(double x, double y) {

    if((3.0 + (mCamera.zoom - y / 5.0f)) > 0.1f)
        mCamera.zoom -= y / 5.0f;
}

void Scene::resetCamera() {
    glm::quat identityQuat;
    mCamera.orientation = identityQuat;
    mCamera.zoom = 2.0;
}