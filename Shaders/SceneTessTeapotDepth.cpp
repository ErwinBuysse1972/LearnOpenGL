#include "SceneTessTeapotDepth.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneTessTeapotDepth::SceneTessTeapotDepth(std::shared_ptr<CTracer> tracer)
    :Scene(tracer)
{
    CFuncTracer trace("SceneTessTeapotDepth::SceneTessTeapotDepth", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::initScene()
{
    CFuncTracer trace("SceneTessTeapotDepth::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        ///////////// Uniforms ////////////////////
        m_prog.setUniform("MinTessLevel", 2);
        m_prog.setUniform("MaxTessLevel", 15);
        m_prog.setUniform("MaxDepth", 20.0f);
        m_prog.setUniform("MinDepth", 2.0f);
        m_prog.setUniform("LineWidth", 0.8f);
        m_prog.setUniform("LineColor", vec4(0.05f, 0.0f, 0.05f, 1.0f));
        m_prog.setUniform("LightPosition", vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("LightIntensity", vec3(1.0f, 1.0f, 1.0f));
        m_prog.setUniform("Kd", vec3(0.9f, 0.9f, 1.0f));
        /////////////////////////////////////////////

        glPatchParameteri(GL_PATCH_VERTICES, 16);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::update(float t)
{
    CFuncTracer trace("SceneTessTeapotDepth::update", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::render()
{
    CFuncTracer trace("SceneTessTeapotDepth::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos(0.0f, 1.0f, 6.25f);
        m_view = glm::lookAt(cameraPos,
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f));

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(-2.0f, 0.0f, 0.0f));
        m_model = glm::translate(m_model, vec3(0.0f, -1.5f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(2.0f, 0.0f, -5.0f));
        m_model = glm::translate(m_model, vec3(0.0f, -1.5f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(7.0f, 0.0f, -10.0f));
        m_model = glm::translate(m_model, vec3(0.0f, -1.5f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(17.0f, 0.0f, -20.0f));
        m_model = glm::translate(m_model, vec3(0.0f, -1.5f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();

        glFinish();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneTessTeapotDepth::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneTessTeapotDepth::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::resize(int w, int h)
{
    CFuncTracer trace("SceneTessTeapotDepth::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);

        float w2 = w / 2.0f;
        float h2 = h / 2.0f;
        m_viewport = mat4(  vec4(w2, 0.0f, 0.0f, 0.0f),
                            vec4(0.0f, h2, 0.0f, 0.0f),
                            vec4(0.0f, 0.0f, 1.0f, 0.0f),
                            vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 30.0f);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::setMatrices(void)
{
    CFuncTracer trace("SceneTessTeapotDepth::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix",mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
        m_prog.setUniform("ViewportMatrix", m_viewport);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapotDepth::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneTessTeapotDepth::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/tessteapotdepth.vert.glsl", GLSLShader::VERTEX);
        m_prog.compileShader("shaders/tessteapotdepth.frag.glsl", GLSLShader::FRAGMENT);
        m_prog.compileShader("shaders/tessteapotdepth.geom.glsl", GLSLShader::GEOMETRY);
        m_prog.compileShader("shaders/tessteapotdepth.tes.glsl", GLSLShader::TESS_EVALUATION);
        m_prog.compileShader("shaders/tessteapotdepth.tcs.glsl", GLSLShader::TESS_CONTROL);
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
