#include <iostream>
using std::cerr;
using std::endl;
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
using glm::vec4;

#include "SceneFog.h"

SceneFog::SceneFog(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_tPrev(0.0f)
    , m_plane(50.0f, 50.0f, 1, 1)
    , m_teapot(14, glm::mat4(1.0f))
{
    CFuncTracer trace("SceneFog::SceneFog", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFog::initScene()
{
    CFuncTracer trace("SceneFog::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(0.0f, 4.0f, 6.0f), vec3(0.0f, 2.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = mat4(1.0f);

        m_angle = 0.0;

        m_prog.setUniform("Light.L", vec3(0.9f));
        m_prog.setUniform("Light.La", vec3(0.2f));
        m_prog.setUniform("Fog.MaxDist", 30.0f);
        m_prog.setUniform("Fog.MinDist", 1.0f);
        m_prog.setUniform("Fog.Density", 0.08f);    // 0.01f-0.1f (low density), 0.1f - 1.0f (medium density), Hight density >1.0f
        m_prog.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFog::update(float t)
{
    CFuncTracer trace("SceneFog::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;

        m_angle += 1.0f * deltaT;
        if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFog::render()
{
    CFuncTracer trace("SceneFog::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec4 lightPos = vec4(10.0f * cos(m_angle), 10.0f, 10.0f * sin(m_angle), 1.0f);
        m_prog.setUniform("Light.Position", m_view * lightPos);

        m_prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
        m_prog.setUniform("Material.Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Shininess", 180.0f);

        float dist = 0.0f;
        for (int i = 0; i < 4; i++) {
            m_model = mat4(1.0f);
            m_model = glm::translate(m_model, vec3(dist * 0.6f - 1.0f, 0.0f, -dist));
            m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
            SetMatrices();
            m_teapot.render();
            dist += 7.0f;
        }

        m_prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
        m_prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Shininess", 180.0f);

        m_model = mat4(1.0f);
        SetMatrices();
        m_plane.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFog::resize(int w, int h)
{
    CFuncTracer trace("SceneFog::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFog::SetMatrices()
{
    CFuncTracer trace("SceneFog::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFog::CompileAndLinkShader()
{
    CFuncTracer trace("SceneFog::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/fog.vert.glsl");
        m_prog.compileShader("shaders/fog.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
