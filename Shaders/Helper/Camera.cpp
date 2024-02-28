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