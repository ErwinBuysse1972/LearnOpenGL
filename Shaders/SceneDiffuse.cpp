#include "SceneDiffuse.h"
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;


SceneDiffuse::SceneDiffuse(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_torus(0.7f, 0.3f, 100, 100)
{
    CFuncTracer trace("SceneDiffuse::SceneDiffuse", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiffuse::initScene()
{
    CFuncTracer trace("SceneDiffuse::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_model = mat4(1.0f);
        m_model = glm::rotate(m_model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(35.0f), vec3(0.0f, 1.0f, 0.0f));
        m_view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = mat4(1.0f);

        m_prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
        m_prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
        m_prog.setUniform("LightPosition", m_view * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiffuse::update(float t)
{
    CFuncTracer trace("SceneDiffuse::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiffuse::render()
{
    CFuncTracer trace("SceneDiffuse::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SetMatrices();
        m_torus.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiffuse::resize(int w, int h)
{
    CFuncTracer trace("SceneDiffuse::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiffuse::SetMatrices(void)
{
    CFuncTracer trace("SceneDiffuse::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix",
        glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP",m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what()); 
        exit(EXIT_FAILURE);
    }
}
void SceneDiffuse::CompileAndLinkShader()
{
    CFuncTracer trace("SceneDiffuse::CompileAndLinkShader", m_trace);
    try 
    {
        m_prog.compileShader("shaders/diffuse.vert.glsl");
        m_prog.compileShader("shaders/diffuse.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        trace.Error("GLSL exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

