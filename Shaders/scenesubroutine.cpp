#include "scenesubroutine.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneSubroutine::SceneSubroutine(std::shared_ptr<CTracer> tracer)
    :Scene(tracer)
    , m_teapot(13, mat4(1.0f))
{
    CFuncTracer trace("SceneSubroutine::SceneSubroutine", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSubroutine::initScene()
{
    CFuncTracer trace("SceneSubroutine::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
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
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSubroutine::update(float t)
{
    CFuncTracer trace("SceneSubroutine::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSubroutine::render()
{
    CFuncTracer trace("SceneSubroutine::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        m_prog.setUniform("Light.Position", lightPos);

        GLuint programHandle = m_prog.getHandle();

        GLuint adsIndex = glGetSubroutineIndex(programHandle,
            GL_VERTEX_SHADER,
            "phongModel");
        GLuint diffuseIndex = glGetSubroutineIndex(programHandle,
            GL_VERTEX_SHADER,
            "diffuseOnly");

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &adsIndex);
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(-3.0, -1.5, 0.0));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        SetMatrices();
        m_teapot.render();

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &diffuseIndex);
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(3.0f, -1.5f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        SetMatrices();
        m_teapot.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSubroutine::resize(int w, int h)
{
    CFuncTracer trace("SceneSubroutine::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(50.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneSubroutine::SetMatrices(void)
{
    CFuncTracer trace("SceneSubroutine::SetMatrices", m_trace);
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
void SceneSubroutine::CompileAndLinkShader(void) 
{
    CFuncTracer trace("SceneSubroutine::CompileAndLinkShader", m_trace);
    try 
    {
        m_prog.compileShader("shaders/subroutine.vert.glsl");
        m_prog.compileShader("shaders/subroutine.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL Exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
