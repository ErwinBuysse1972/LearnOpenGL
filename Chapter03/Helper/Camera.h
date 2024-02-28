#pragma once
#include <memory>
#include "cfunctracer.h"
#include "glm/glm.hpp"

class Camera
{
public:
    Camera(std::shared_ptr<CTracer> tracer);

    glm::mat4 getViewMatrix() const;
    virtual void rotate(float yawl, float pitch) = 0;


protected:
    glm::vec3 m_Position;
    glm::vec3 m_targetPosition;
    glm::vec3 m_Up;
    
    // Euler angles (As radians)
    float m_Pitch;
    float m_Yaw;

    std::shared_ptr<CTracer> m_trace;
};

class OrbitCamera : public Camera
{
public:
    OrbitCamera(std::shared_ptr<CTracer> tracer);

    virtual void rotate(float yawl, float pitch); // angles are passed in degrees
    void setLookAt(glm::vec3 target);
    void setRadius(float radius);
    void setPosition(glm::vec3 position);

private:
    void updateCameraVectors();
    float m_Radius;

};