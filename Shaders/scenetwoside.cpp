#include "scenetwoside.h"
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;


SceneTwoSite::SceneTwoSite(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_teapot(13, glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f)))
{
    CFuncTracer trace("SceneTwoSite::SceneTwoSite", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneTwoSite::initScene()
{
    CFuncTracer trace("SceneTwoSite::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(2.0f, 4.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
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
void SceneTwoSite::update(float t)
{
    CFuncTracer trace("SceneTwoSite::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneTwoSite::render()
{
    CFuncTracer trace("SceneTwoSite::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::vec4 worldLight = glm::vec4(2.0f, 4.0f, 2.0f, 1.0f);
        m_model = glm::rotate(mat4(1.0f), glm::radians(m_angle), vec3(0.0f, 1.0f, 0.0f));
        m_prog.setUniform("Light.Position", m_view * m_model * worldLight);

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0, -1.0, 0.0));
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
void SceneTwoSite::resize(int w, int h)
{
    CFuncTracer trace("SceneTwoSite::resize", m_trace);
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
void SceneTwoSite::SetMatrices(void)
{
    CFuncTracer trace("SceneTwoSite::SetMatrices", m_trace);
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
void SceneTwoSite::CompileAndLinkShader(void)
{
    CFuncTracer trace("SceneTwoSite::CompileAndLinkShader", m_trace);
    try 
    {
        //m_prog.compileShader("shaders/twoside.vert.glsl");
        //m_prog.compileShader("shaders/twoside.frag.glsl");
        m_prog.compileShader("shaders/twoside_conditional.vert.glsl");
        m_prog.compileShader("shaders/twoside_conditional.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
