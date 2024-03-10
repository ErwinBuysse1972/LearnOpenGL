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

    virtual glm::mat4 getViewMatrix() const;
    virtual void rotate(float yaw, float pitch) {}  // in degrees

    glm::vec3 getPosition(void) { return m_Position; }
    glm::vec3 getTarget(void) { return m_TargetPos; }
    glm::vec3 getUp(void) { return m_Up; }

    virtual float getYaw(void) { return m_Yaw; }
    virtual float getPitch(void) { return m_Pitch; }

    virtual void setTarget(const glm::vec3& target) { m_TargetPos = target; }
    virtual void setPosition(const glm::vec3& pos) { m_Position = pos; }
    virtual void setUp(const glm::vec3& up) { m_Up = up; }
    virtual void setYaw(const float& yaw) { m_Yaw = yaw; }
    virtual void setPitch(const float& pitch) { m_Pitch = pitch; }

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

    virtual void setPosition(const glm::vec3& position) override;
    virtual void rotate(float yaw, float pitch);    // in degrees

    // Camera Controls
    void setLookAt(const glm::vec3& target);
    void setRadius(float radius);
protected:
    void updateCameraVectors() override;
private:
    float m_Radius;

    
};


enum class CameraMovement
{
    forward = 1,
    backward = 2,
    left = 4,
    right = 8
};
enum class KeyboardMode
{
    eRotation,
    eMovement
};
class CameraArcBall : public Camera
{
public:
    CameraArcBall(std::shared_ptr<CTracer> tracer, 
                  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
                  glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), 
                  glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), 
                  float yaw = -90.0f, 
                  float pitch = 45.0f);
    CameraArcBall(std::shared_ptr<CTracer> tracer, 
                  float posX, float posY, float posZ, 
                  float upX, float upY, float upZ, 
                  float yaw, 
                  float pitch);

    glm::mat4 getViewMatrix() const override;
    void processKeyboard(CameraMovement eMovement, float dt);
    void processMouseRotation(float dx, float dy, bool constrainPitch = true);
    void processMouseMove(float dx, float dy);
    void processMouseScroll(float yoffset);
    void rotate(float yaw, float pictch) override;

    virtual glm::vec3 getRight(void) { return m_Right; }
    virtual glm::vec3 getFront(void) { return m_Front; }
    virtual glm::vec3 getUp(void) { return m_WorldUp; }
    virtual float getMouseSensitivity(void) { return m_MouseSensitivity; }
    virtual float getMovementSpeed(void) { return m_MovementSpeed; }
    virtual float getZoom(void) { return m_Zoom; }
    virtual KeyboardMode getKbdMode(void) { return m_kbdMode; }

    virtual void setRight(const glm::vec3& right) { m_Right = right; }
    virtual void setFront(const glm::vec3& front) { m_Front = front; }
    virtual void setUp(const glm::vec3& up) { m_WorldUp = up; }
    virtual void setMouseSensitivity(float sens) { m_MouseSensitivity = sens; }
    virtual void setMovementSpeed(float speed) { m_MovementSpeed = speed; }
    virtual void setZoom(float zoom) { m_Zoom = zoom; }
    virtual void setKbdMode(KeyboardMode mode) { m_kbdMode = mode; }
protected:
    void updateCameraVectors() override;

private:
    glm::vec3 m_Right;
    glm::vec3 m_Front;
    glm::vec3 m_WorldUp;
    KeyboardMode m_kbdMode;

    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;

};
#endif //CAMERA_H
