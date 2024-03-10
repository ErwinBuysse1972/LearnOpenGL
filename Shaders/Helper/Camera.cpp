//-----------------------------------------------------------------------------
// Camera.cpp by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Basic camera class including derived orbit-style camera support
//-----------------------------------------------------------------------------
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

//------------------------------------------------------------
// Base Camera class constructor
//------------------------------------------------------------
Camera::Camera(std::shared_ptr<CTracer> tracer)
    : m_trace(tracer)
    , m_Position(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_TargetPos(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_Up(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_Yaw(0.0f)
    , m_Pitch(0.0f)
{
}

//------------------------------------------------------------
// Base Camera - Returns view matrix
//------------------------------------------------------------
glm::mat4 Camera::getViewMatrix()const
{
    return glm::lookAt(m_Position, m_TargetPos, m_Up);
}

//------------------------------------------------------------
// OrbitCamera - constructor
//------------------------------------------------------------
OrbitCamera::OrbitCamera(std::shared_ptr<CTracer> tracer)
    : Camera(tracer)
    , m_Radius(10.0f)
{}

//------------------------------------------------------------
// OrbitCamera - Sets the target to look at
//------------------------------------------------------------
void OrbitCamera::setLookAt(const glm::vec3& target)
{
    m_TargetPos = target;
}

//------------------------------------------------------------
// OrbitCamera - Sets the radius of camera to target distance
//------------------------------------------------------------
void OrbitCamera::setRadius(float radius)
{
    // Clamp the radius
    m_Radius = glm::clamp(radius, 2.0f, 80.0f);
}

//------------------------------------------------------------
// OrbitCamera - Sets the world position of the camera
//------------------------------------------------------------
void OrbitCamera::setPosition(const glm::vec3& position)
{
    m_Position = position;
}

//------------------------------------------------------------
// OrbitCamera - Rotates the camera around the target look
// at position given yaw and pitch in degrees.
//------------------------------------------------------------
void OrbitCamera::rotate(float yaw, float pitch)
{
    m_Yaw = glm::radians(yaw);
    m_Pitch = glm::radians(pitch);

    m_Pitch = glm::clamp(m_Pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

//------------------------------------------------------------
// OrbitCamera - Calculates the front vector from the Camera's
// (updated) Euler Angles
//------------------------------------------------------------
void OrbitCamera::updateCameraVectors()
{
    // Spherical to Cartesian coordinates
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)

    m_Position.x = m_TargetPos.x + m_Radius * cosf(m_Pitch) * sinf(m_Yaw);
    m_Position.y = m_TargetPos.y + m_Radius * sinf(m_Pitch);
    m_Position.z = m_TargetPos.z + m_Radius * cosf(m_Pitch) * cosf(m_Yaw);
}


CameraArcBall::CameraArcBall(std::shared_ptr<CTracer> tracer,
    glm::vec3 position,
    glm::vec3 target,
    glm::vec3 up,
    float yaw,
    float pitch)
    : Camera(tracer)
    , m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_MovementSpeed(2.5f)
    , m_MouseSensitivity(0.1f)
    , m_Zoom(45.0f)
    , m_kbdMode(KeyboardMode::eRotation)
{
    CFuncTracer trace("CameraArcBall::CameraArcBall", m_trace);
    try
    {
        m_Position = position;
        m_TargetPos = target;
        m_WorldUp = up;
        m_Up = glm::vec3(0.0f, 0.0f, 1.0f);
        m_Yaw = -90.0f;
        m_Pitch = 45.0f;
        updateCameraVectors();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
CameraArcBall::CameraArcBall(std::shared_ptr<CTracer> tracer,
    float posX, float posY, float posZ,
    float upX, float upY, float upZ,
    float yaw,
    float pitch)
    :Camera(tracer)
    , m_kbdMode(KeyboardMode::eRotation)
{
    CFuncTracer trace("CameraArcBall::CameraArcBall", m_trace);
    try
    {
        m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_Position = glm::vec3(posX, posY, posZ);
        m_WorldUp = glm::vec3(upX, upY, upZ);
        m_MovementSpeed = 2.5f;
        m_MouseSensitivity = 0.1f;
        m_Zoom = 45.0f;
        updateCameraVectors();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void CameraArcBall::processKeyboard(CameraMovement eMovement, float dt)
{
    CFuncTracer trace("CameraArcBall::processKeyboard", m_trace);
    try
    {
//        m_MovementSpeed = 10.0f;
        float velocity = m_MovementSpeed * dt;
        if (m_kbdMode == KeyboardMode::eRotation)
        {
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::forward)) m_Pitch += velocity;
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::backward)) m_Pitch -= velocity;
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::left)) m_Yaw += velocity;
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::right)) m_Yaw -= velocity;

            if (m_Pitch > 89.0f) m_Pitch = 89.0f;
            if (m_Pitch < -89.0f) m_Pitch = -89.0f;
            updateCameraVectors();
        }
        else if (m_kbdMode == KeyboardMode::eMovement)
        {
 
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::forward)) m_Position += velocity * m_Front;
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::backward)) m_Position -= velocity * m_Front;
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::left)) m_Position -= velocity * glm::normalize(glm::cross(m_Front, m_Up));
            if (static_cast<int>(eMovement) & static_cast<int>(CameraMovement::right)) m_Position += velocity * glm::normalize(glm::cross(m_Front, m_Up));
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CameraArcBall::processMouseRotation(float dx, float dy, bool constrainPitch)
{
    CFuncTracer trace("CameraArcBall::processMouseRotation", m_trace);
    try
    {
        dx *= m_MouseSensitivity;
        dy *= m_MouseSensitivity;

        m_Yaw += dx;
        m_Pitch += dy;

        if (constrainPitch)
        {
            if (m_Pitch > 89.0f) m_Pitch = 89.0f;
            if (m_Pitch < -89.0f) m_Pitch = -89.0f;
        }
        updateCameraVectors();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CameraArcBall::processMouseMove(float dx, float dy)
{
    CFuncTracer trace("CameraArcBall::processMouseMove", m_trace);
    try
    {
        dx *= m_MouseSensitivity;
        dy *= m_MouseSensitivity;

        glm::mat3 invView = glm::inverse(getViewMatrix());
        m_TargetPos -= dx * invView * glm::vec3(1.0f, 0.0f, 0.0f);
        m_TargetPos -= dy * invView * glm::vec3(0.0f, 1.0f, 0.0f);

        updateCameraVectors();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CameraArcBall::processMouseScroll(float yoffset)
{
    CFuncTracer trace("CameraArcBall::processMouseScroll", m_trace);
    try
    {
        m_Position += m_Front * yoffset * 8.0f;
        updateCameraVectors();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CameraArcBall::rotate(float yaw, float pitch)
{
    CFuncTracer trace("CameraArcBall::rotate", m_trace, false);
    try
    {
        trace.Trace("yaw : %.8f, pitch : %.8f", m_Yaw, m_Pitch);
        m_Yaw = glm::radians(yaw);
        m_Pitch = glm::radians(pitch);
        updateCameraVectors();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CameraArcBall::updateCameraVectors()
{
    CFuncTracer trace("CameraArcBall::updateCameraVectors", m_trace, false);
    try
    {
        float radius = glm::distance(m_Position, m_TargetPos);
        m_Position.x = m_TargetPos.x + radius * sin(m_Yaw) * sin(m_Pitch);
        m_Position.y = m_TargetPos.y + radius * cos(m_Yaw) * sin(m_Pitch);
        m_Position.z = m_TargetPos.z + radius * cos(m_Pitch);

        m_Up = m_WorldUp;
        m_Front = glm::normalize(m_TargetPos - m_Position);
        m_Right = glm::normalize(glm::cross(m_Front, m_Up));

        trace.Trace("Position (%.8f, %.8f, %.8f), Target (%.8f, %.8f, %.8f)", m_Position.x, m_Position.y, m_Position.z, m_TargetPos.x, m_TargetPos.y, m_TargetPos.z);
        
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
glm::mat4 CameraArcBall::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
}