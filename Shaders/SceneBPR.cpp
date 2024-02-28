#include <iostream>
using std::cerr;
using std::endl;
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include "SceneBPR.h"

SceneBPR::SceneBPR(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_plane(20.0f, 20.0f, 1, 1)
    , m_teapot(5, glm::mat4(1.0f))
    , m_tPrev(0.0f)
    , m_lightPos(5.0f, 5.0f, 5.0f, 1.0f)
{
    CFuncTracer trace("SceneBPR::SceneBPR", m_trace);
    try
    {
        m_mesh = ObjMesh::load("../media/spot/spot_triangulated.obj");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::initScene()
{
    CFuncTracer trace("SceneBPR::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(
            glm::vec3(0.0f, 4.0f, 7.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        m_projection = glm::perspective(glm::radians(50.0f), (float)m_width / m_height, 0.5f, 100.0f);

        m_lightAngle = 0.0f;
        m_lightRotationSpeed = 1.5f;

        m_prog.setUniform("Light[0].L", glm::vec3(45.0f));
        m_prog.setUniform("Light[0].Position", m_view * m_lightPos);
        m_prog.setUniform("Light[1].L", glm::vec3(0.3f));
        m_prog.setUniform("Light[1].Position", glm::vec4(0, 0.15f, -1.0f, 0));
        m_prog.setUniform("Light[2].L", glm::vec3(45.0f));
        m_prog.setUniform("Light[2].Position", m_view * glm::vec4(-7, 3, 7, 1));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::update(float t)
{
    CFuncTracer trace("SceneBPR::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;
        if (animating()) {
            m_lightAngle = glm::mod(m_lightAngle + deltaT * m_lightRotationSpeed, glm::two_pi<float>());
            m_lightPos.x = glm::cos(m_lightAngle) * 7.0f;
            m_lightPos.y = 3.0f;
            m_lightPos.z = glm::sin(m_lightAngle) * 7.0f;
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::render()
{
    CFuncTracer trace("SceneBPR::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_prog.setUniform("Light[0].Position", m_view * m_lightPos);
        DrawScene();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::resize(int w, int h)
{
    CFuncTracer trace("SceneBPR::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::SetMatrices()
{
    CFuncTracer trace("SceneBPR::SetMatrices", m_trace);
    try
    {
        glm::mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(mv));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::CompileAndLinkShader()
{
    CFuncTracer trace("SceneBPR::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/pbr.vert.glsl");
        m_prog.compileShader("shaders/pbr.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        std::cerr << e.what() << std::endl;
        trace.Error("GLSL program exception : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::DrawScene()
{
    CFuncTracer trace("SceneBPR::DrawScene", m_trace);
    try
    {
        DrawFloor();

        // Draw dielectric cows with varying roughness
        int numCows = 9;
        glm::vec3 cowBaseColor(0.1f, 0.33f, 0.17f);
        for (int i = 0; i < numCows; i++) {
            float cowX = i * (10.0f / (numCows - 1)) - 5.0f;
            float rough = (i + 1) * (1.0f / numCows);
            DrawSpot(glm::vec3(cowX, 0, 0), rough, 0, cowBaseColor);
        }

        // Draw metal cows
        float metalRough = 0.43f;
        // Gold
        DrawSpot(glm::vec3(-3.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(1, 0.71f, 0.29f));
        // Copper
        DrawSpot(glm::vec3(-1.5f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.64f, 0.54f));
        // Aluminum
        DrawSpot(glm::vec3(-0.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.91f, 0.92f, 0.92f));
        // Titanium
        DrawSpot(glm::vec3(1.5f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.542f, 0.497f, 0.449f));
        // Silver
        DrawSpot(glm::vec3(3.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.93f, 0.88f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::DrawFloor()
{
    CFuncTracer trace("SceneBPR::DrawFloor", m_trace);
    try
    {
        m_model = glm::mat4(1.0f);
        m_prog.setUniform("Material.Rough", 0.9f);
        m_prog.setUniform("Material.Metal", 0);
        m_prog.setUniform("Material.Color", glm::vec3(0.2f));
        m_model = glm::translate(m_model, glm::vec3(0.0f, -0.75f, 0.0f));

        SetMatrices();
        m_plane.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneBPR::DrawSpot(const glm::vec3& pos, float rough, int metal, const glm::vec3& color)
{
    CFuncTracer trace("SceneBPR::DrawSpot", m_trace);
    try
    {
        m_model = glm::mat4(1.0f);
        m_prog.setUniform("Material.Rough", rough);
        m_prog.setUniform("Material.Metal", metal);
        m_prog.setUniform("Material.Color", color);
        m_model = glm::translate(m_model, pos);
        m_model = glm::rotate(m_model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        SetMatrices();
        m_mesh->render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
