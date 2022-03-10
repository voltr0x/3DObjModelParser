#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

Camera& Camera::Instance(){
    static Camera* instance = new Camera();
    return *instance;
}

void Camera::MouseLook(int mouseX, int mouseY){
    // TODO: Which vector are you modifying?
	// *hint* not the eye...
}

void Camera::MoveForward(float speed){
    // TODO: Which vector are you modifying?
}

void Camera::MoveBackward(float speed){
    // TODO: Which vector are you modifying?
}

void Camera::MoveLeft(float speed){
    // TODO: Which vector are you modifying?
}

void Camera::MoveRight(float speed){
    // TODO: Which vector are you modifying?
}

void Camera::MoveUp(float speed){
    // TODO: Which vector are you modifying?
}

void Camera::MoveDown(float speed){
    // TODO: Which vector are you modifying?
}

float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}

Camera::Camera(){
    std::cout << "Created a Single Camera!\n";
	// Position us at the origin.
    m_eyePosition = glm::vec3(0.0f,0.0f, 0.0f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    m_viewDirection = glm::vec3(0.0f,0.0f, -1.0f);
	// For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
