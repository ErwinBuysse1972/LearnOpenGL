#include <iostream>
using std::cerr;
using std::endl;
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include "SceneDirectional.h"

SceneDirectional::SceneDirectional(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_torus(0.7f, 0.3f, 50, 50)
{
    CFuncTracer trace("SceneDirectional::SceneDirectional", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDirectional::initScene()
{
    CFuncTracer trace("SceneDirectional::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = mat4(1.0f);

        m_prog.setUniform("Light.Position", m_view * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        m_prog.setUniform("Light.L", vec3(0.8f, 0.8f, 0.8f));
        m_prog.setUniform("Light.La", vec3(0.1f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDirectional::update(float t)
{
    CFuncTracer trace("SceneDirectional::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDirectional::render()
{
    CFuncTracer trace("SceneDirectional::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_prog.setUniform("Material.Kd", 0.8f, 0.8f, 0.8f);
        m_prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        m_prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
        m_prog.setUniform("Material.Shininess", 180.0f);

        m_model = mat4(1.0f);
        m_model = glm::rotate(m_model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
        SetMatrices();
        m_torus.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDirectional::resize(int w, int h)
{
    CFuncTracer trace("SceneDirectional::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDirectional::SetMatrices()
{
    CFuncTracer trace("SceneDirectional::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix",
            glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDirectional::CompileAndLinkShader()
{
    CFuncTracer trace("SceneDirectional::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/directional.vert.glsl");
        m_prog.compileShader("shaders/directional.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
