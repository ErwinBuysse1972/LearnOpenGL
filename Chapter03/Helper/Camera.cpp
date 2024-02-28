#include "Camera.h"
#include "glm/gtx/transform.hpp"

Camera::Camera(std::shared_ptr<CTracer> tracer)
    : m_trace(tracer)
    , m_Position(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_targetPosition(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_Up(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_Pitch(0.0f)
    , m_Yaw(0.0f)

{
    CFuncTracer trace("Camera::Camera", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}

glm::mat4 Camera::getViewMatrix() const
{
    CFuncTracer trace("Camera::getViewMatrix", m_trace);
    try
    {
        return glm::lookAt(m_Position, m_targetPosition, m_Up);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

OrbitCamera::OrbitCamera(std::shared_ptr<CTracer> tracer)
    : Camera(tracer)
    , m_Radius(10.0f)
{
    CFuncTracer trace("OrbitCamera::OrbitCamera", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void OrbitCamera::rotate(float yawl, float pitch)
{
    CFuncTracer trace("OrbitCamera::rotate", m_trace);
    try
    {
        m_Yaw = glm::radians(yawl);
        m_Pitch = glm::radians(pitch);

        m_Pitch = glm::clamp(pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>()/ 2.0f - 0.1f);

        updateCameraVectors();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void OrbitCamera::setLookAt(glm::vec3 target)
{
    CFuncTracer trace("OrbitCamera::setLookAt", m_trace);
    try
    {
        m_targetPosition = target;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void OrbitCamera::setRadius(float radius)
{
    CFuncTracer trace("OrbitCamera::setRadius", m_trace);
    try
    {
        m_Radius = glm::clamp(radius, 2.0f, 80.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void OrbitCamera::setPosition(glm::vec3 position)
{
    CFuncTracer trace("OrbitCamera::setPosition", m_trace);
    try
    {
        m_Position = position;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void OrbitCamera::updateCameraVectors()
{
    CFuncTracer trace("OrbitCamera::updateCameraVectors", m_trace);
    try
    {
        m_Position.x = m_Position.x + m_Radius * cosf(m_Pitch) * sinf(m_Yaw);
        m_Position.y = m_Position.y + m_Radius * sinf(m_Pitch);
        m_Position.z = m_Position.z + m_Radius * cosf(m_Pitch) * cosf(m_Yaw);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}

