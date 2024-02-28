//-----------------------------------------------------------------------------
// Camera.h by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Basic camera class including derived orbit-style camera support
//-----------------------------------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

#include "cfunctracer.h"
#include "glm/glm.hpp"

//--------------------------------------------------------------
// Camera Base Class
//--------------------------------------------------------------
class Camera
{
public:

    virtual void setPosition(const glm::vec3& position) {}
    glm::mat4 getViewMatrix() const;
    virtual void rotate(float yaw, float pitch) {}  // in degrees

protected:
    std::shared_ptr<CTracer> m_trace;
    glm::vec3 m_Position;
    glm::vec3 m_TargetPos;
    glm::vec3 m_Up;
    float m_Yaw;                    // Euler Angles (in radians)
    float m_Pitch;                  // Euler Angles (in radians)

    Camera(std::shared_ptr<CTracer> tracer);
    virtual void updateCameraVectors() {}
};

//--------------------------------------------------------------
// Orbit Camera Class
//--------------------------------------------------------------
class OrbitCamera : public Camera
{
public:

    OrbitCamera(std::shared_ptr<CTracer> tracer);

    virtual void setPosition(const glm::vec3& position);
    virtual void rotate(float yaw, float pitch);    // in degrees

    // Camera Controls
    void setLookAt(const glm::vec3& target);
    void setRadius(float radius);

private:
    float m_Radius;

    void updateCameraVectors();
};
#endif //CAMERA_H
