/******************************************************************************
 *
 * A class for handling of all controls 
 *
 *****************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#define WIDTH 1024
#define HEIGHT 768

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
 #include <glm/gtc/matrix_inverse.hpp>
 #include "glm/gtc/matrix_transform.hpp"
#include <iostream>

class Camera{

public:

    Camera(glm::vec3 p = glm::vec3(0.0f, 0.0f, 2.0f));

    ~Camera();

    void update();

    void dragStart(double x, double y);
    
    void dragUpdate(double x, double y);
    
    void dragEnd();
    
    bool dragged() const { return mDragged; }

    glm::quat& rotate(glm::quat &orientation, double x, double y);

    glm::vec2 direction(double x, double y);

    void center(float x, float y) { mCenterPosition.x = x; mCenterPosition.y = y; }

    void reset();

    glm::mat4 getModelMatrix() { return mModelMatrix; }

    glm::mat4 getViewMatrix() { return mViewMatrix; }

    glm::mat4 getProjectionMatrix() { return mProjectionMatrix; }

    glm::vec3 getPosition() { return mPosition; }

    glm::quat& getOrientation() { return mOrientation; }

    float getZoom() { return mZoom; }

    void setModelMatrix(glm::mat4 M) { mModelMatrix = M; }

    void setViewMatrix(glm::mat4 V) { mViewMatrix = V; }

    void setProjectionMatrix(glm::mat4 P) { mProjectionMatrix = P; }

    void setOrientation(glm::quat O) { mOrientation = O; }

    void setZoom(float z) { mZoom = z; }

private:

    //instance variables
    glm::vec2 mCenterPosition;
    
    glm::vec2 mDragStartPosition;

    glm::mat4 mModelMatrix;

    glm::mat4 mViewMatrix;

    glm::mat4 mProjectionMatrix;

    glm::vec3 mPosition;

    glm::quat mOrientation;

    float mZoom;
    
    float mRadius;
    
    bool mDragged;


    // Functions
    glm::vec3 map_to_sphere(const glm::vec2 &point);
};

#endif // CAMERA_H