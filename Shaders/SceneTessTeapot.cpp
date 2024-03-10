#include "SceneTessTeapot.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include <cfunctracer.h>
#include <memory>

SceneTessTeapot::SceneTessTeapot(std::shared_ptr<CTracer>tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
    , m_tessLvl(4)
    , m_lineWidth(0.8f)

{
    CFuncTracer trace("SceneTessTeapot::SceneTessTeapot", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapot::initScene()
{
    CFuncTracer trace("SceneTessTeapot::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        m_angle = glm::pi<float>() / 3.0f;

        ///////////// Uniforms ////////////////////
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
void SceneTessTeapot::update(float t)
{
    CFuncTracer trace("SceneTessTeapot::update", m_trace);
    try
    {
        ///////////// Uniforms ////////////////////
        m_prog.setUniform("TessLevel", m_tessLvl);
        m_prog.setUniform("LineWidth", m_lineWidth);
        /////////////////////////////////////////////

        float deltaT = t - m_tPrev;
        if (m_tPrev == 0) deltaT = 0;
        m_tPrev = t;
        m_angle += m_rotSpeed * deltaT;
        if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_over_pi<float>();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapot::render()
{
    CFuncTracer trace("SceneTessTeapot::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        vec3 cameraPos(4.25f * cos(m_angle), 3.0f, 4.25f * sin(m_angle));
        m_view = glm::lookAt(cameraPos,
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f));

        m_model = mat4(1.0f);
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
void SceneTessTeapot::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneTessTeapot::renderGuiWindow", m_trace);
    try
    {
        ImGui::Begin("Scene Quad 3D parameters");
        ImGui::SliderInt("Tesselation level", &m_tessLvl, 1, 100);
        ImGui::SliderFloat("Line width", &m_lineWidth, 0.1f, 3.0f);
        ImGui::End();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapot::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneTessTeapot::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapot::resize(int w, int h)
{
    CFuncTracer trace("SceneTessTeapot::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        float w2 = w / 2.0f;
        float h2 = h / 2.0f;
        m_viewport = mat4(vec4( w2, 0.0f, 0.0f, 0.0f),
                                vec4(0.0f, h2, 0.0f, 0.0f),
                                vec4(0.0f, 0.0f, 1.0f, 0.0f),
                                vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
        m_width = w;
        m_height = h;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapot::setMatrices(void)
{
    CFuncTracer trace("SceneTessTeapot::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
        m_prog.setUniform("ViewportMatrix", m_viewport);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneTessTeapot::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneTessTeapot::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/tessteapot.vert.glsl", GLSLShader::VERTEX);
        m_prog.compileShader("shaders/tessteapot.frag.glsl", GLSLShader::FRAGMENT);
        m_prog.compileShader("shaders/tessteapot.geom.glsl", GLSLShader::GEOMETRY);
        m_prog.compileShader("shaders/tessteapot.tes.glsl", GLSLShader::TESS_EVALUATION);
        m_prog.compileShader("shaders/tessteapot.tcs.glsl", GLSLShader::TESS_CONTROL);
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
