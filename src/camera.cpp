#include "camera.h"

/// --- Movement ---
void Camera::moveForward(const float &deltaTime) {
   cameraPos += (cameraSpeed * deltaTime) * cameraFront;
}
void Camera::moveBackward(const float &deltaTime) {
   cameraPos -= (cameraSpeed * deltaTime) * cameraFront;
}
void Camera::moveLeft(const float &deltaTime) {
   cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                (cameraSpeed * deltaTime);
}
void Camera::moveRight(const float &deltaTime) {
   cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                (cameraSpeed * deltaTime);
}

/// --- Direction ---
void Camera::setDirection(const float &xoffset, const float &yoffset) {
   yaw += xoffset * sensitivity;
   pitch += yoffset * sensitivity;

   if (pitch > 89.0f)
      pitch = 89.0f;
   if (pitch < -89.0f)
      pitch = -89.0f;

   glm::vec3 direction;
   direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   direction.y = sin(glm::radians(pitch));
   direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   cameraFront = glm::normalize(direction);
}

/// --- Projection ---
void Camera::setZoom(const float &yoffset) {
   fov -= yoffset;
   if (fov < 1.0f)
      fov = 1.0f;
   if (fov > 45.0f)
      fov = 45.0f;
}

glm::vec3 Camera::getPos() const { return cameraPos; }

glm::mat4 Camera::getView() const {
   return glm::lookAt(cameraPos,               // Camera position
                      cameraPos + cameraFront, // Camera target
                      cameraUp);               // Upwards vector
}
glm::mat4 Camera::getProjection(const float &aspect, const float &nearPlane,
                                const float &farPlane) const {
   return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}
