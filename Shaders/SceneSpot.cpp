#include <iostream>
using std::cerr;
using std::endl;
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::mat3;
#include "GLUtils.h"


#include "SceneSpot.h"

SceneSpot::SceneSpot(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_plane(50.0f, 50.0f, 1, 1)
    , m_teapot(14, glm::mat4(1.0f))
    , m_torus(1.75f * 0.75f, 0.75f * 0.75f, 50, 50)
{
    CFuncTracer trace("SceneSpot::SceneSpot", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSpot::initScene()
{
    CFuncTracer trace("SceneSpot::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = mat4(1.0f);

        m_angle = 0.0;

        m_prog.setUniform("Spot.L", vec3(0.9f));
        m_prog.setUniform("Spot.La", vec3(0.5f));
        m_prog.setUniform("Spot.Exponent", 50.0f);
        m_prog.setUniform("Spot.Cutoff", glm::radians(15.0f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSpot::update(float t)
{
    CFuncTracer trace("SceneSpot::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;

        m_angle += 0.25f * deltaT;
        if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSpot::render()
{
    CFuncTracer trace("SceneSpot::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        vec4 lightPos = vec4(10.0f * cos(m_angle), 10.0f, 10.0f * sin(m_angle), 1.0f);
        m_prog.setUniform("Spot.Position", vec3(m_view * lightPos));
        mat3 normalMatrix = mat3(vec3(m_view[0]), vec3(m_view[1]), vec3(m_view[2]));
        m_prog.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));

        m_prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
        m_prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        m_prog.setUniform("Material.Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
        m_prog.setUniform("Material.Shininess", 100.0f);

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, 0.0f, -2.0f));
        m_model = glm::rotate(m_model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        SetMatrices();
        m_teapot.render();

        m_prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
        m_prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        m_prog.setUniform("Material.Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
        m_prog.setUniform("Material.Shininess", 100.0f);

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(-1.0f, 0.75f, 3.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        SetMatrices();
        m_torus.render();

        m_prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
        m_prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        m_prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        m_prog.setUniform("Material.Shininess", 180.0f);

        m_model = mat4(1.0f);
        SetMatrices();
        m_plane.render();
        GLUtils::checkForOpenGLError(__FILE__, __LINE__);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSpot::resize(int w, int h)
{
    CFuncTracer trace("SceneSpot::resize", m_trace);
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
void SceneSpot::SetMatrices()
{
    CFuncTracer trace("", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSpot::CompileAndLinkShader()
{
    CFuncTracer trace("SceneSpot::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/spot.vert.glsl");
        m_prog.compileShader("shaders/spot.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL Program exception : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}


