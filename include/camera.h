//===-- camera.h - Camera class definition -------*- C++ -*-===//
//
// Part of the 3d.view project
// Author: BillisC
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the Camera class, which is responsible
/// for camera vector math including movement, direction and projection
///
//===----------------------------------------------------------------------===//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
   glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
   glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
   glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

   float yaw = -90.0f;
   float pitch = 0.0f;
   float fov = 45.0f;

   const float cameraSpeed = 2.3f;
   const float sensitivity = 0.1f;

 public:
   Camera() = default;
   Camera(const float fov, const glm::vec3 cameraPos,
          const glm::vec3 cameraFront)
       : fov(fov), cameraPos(cameraPos), cameraFront(cameraFront) {}

   /// --- Movement ---
   void moveForward(const float &deltaTime);
   void moveBackward(const float &deltaTime);
   void moveLeft(const float &deltaTime);
   void moveRight(const float &deltaTime);

   /// --- Direction ---
   void setDirection(const float &xoffset, const float &yoffset);

   /// --- Projection ---
   void setZoom(const float &yoffset);
   glm::mat4 getView();
   glm::mat4 getProjection(const float &width, const float &height,
                           const float &nearPlane, const float &farPlane);
};

#endif