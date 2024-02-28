#include <iostream>
using std::cerr;
using std::endl;
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

#include "SceneDiscard.h"

SceneDiscard::SceneDiscard(std::shared_ptr<CTracer> tracer)
    :Scene(tracer)
    , m_teapot(13, mat4(1.0f))
{
    CFuncTracer trace("SceneDiscard::SceneDiscard", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiscard::initScene()
{
    CFuncTracer trace("SceneDiscard::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(0.0f, 0.0f, 7.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = mat4(1.0f);

        m_prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
        m_prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
        m_prog.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
        m_prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
        m_prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
        m_prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
        m_prog.setUniform("Material.Shininess", 100.0f);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiscard::update(float t)
{
    CFuncTracer trace("SceneDiscard::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiscard::render()
{
    CFuncTracer trace("SceneDiscard::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        m_prog.setUniform("Light.Position", lightPos);

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0, -1.5, 0.0));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        SetMatrices();
        m_teapot.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiscard::resize(int w, int h)
{
    CFuncTracer trace("SceneDiscard::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(50.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiscard::SetMatrices()
{
    CFuncTracer trace("SceneDiscard::SetMatrices", m_trace);
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
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneDiscard::CompileAndLinkShader()
{
    CFuncTracer trace("SceneDiscard::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/discard.vert.glsl");
        m_prog.compileShader("shaders/discard.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        trace.Error("GLSL Program exception :%s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
