#include "SceneShadeWire.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include <cfunctracer.h>

SceneShadeWire::SceneShadeWire(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
{
    CFuncTracer trace("SceneShadeWire::SceneShadeWire", m_trace);
    try
    {
        m_ogre = ObjMesh::load("../media/bs_ears.obj");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::initScene()
{
    CFuncTracer trace("SceneShadeWire::initScene", m_trace);
    try
    {
        compileAndLinkShader();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        float c = 1.5f;
        m_projection = glm::ortho(-0.4f * c, // left
                                   0.4f * c, // right
                                  -0.3f * c, // bottom
                                   0.3f * c, // top
                                   0.1f,     // near
                                   100.0f);  // far

        ///////////// Uniforms ////////////////////
        m_prog.setUniform("Line.Width", 0.75f);
        m_prog.setUniform("Line.Color", vec4(0.05f, 0.0f, 0.05f, 1.0f));
        m_prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);

        m_prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
        m_prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        m_prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
        m_prog.setUniform("Material.Shininess", 100.0f);
        /////////////////////////////////////////////
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::update(float t)
{
    CFuncTracer trace("SceneShadeWire::update", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::render()
{
    CFuncTracer trace("SceneShadeWire::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos(0.0f, 0.0f, 3.0f);
        m_view = glm::lookAt(cameraPos,
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f));

        m_model = mat4(1.0f);
        setMatrices();
        m_ogre->render();

        glFinish();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneShadeWire::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneShadeWire::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::resize(int w, int h)
{
    CFuncTracer trace("SceneShadeWire::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);

        float w2 = w / 2.0f;
        float h2 = h / 2.0f;
        m_viewport = mat4(vec4(w2, 0.0f, 0.0f, 0.0f),
                          vec4(0.0f, h2, 0.0f, 0.0f),
                          vec4(0.0f, 0.0f, 1.0f, 0.0f),
                          vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
        m_width = w;
        m_height = h;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::setMatrices(void)
{
    CFuncTracer trace("SceneShadeWire::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
        m_prog.setUniform("ViewportMatrix", m_viewport);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneShadeWire::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneShadeWire::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/shadewire.vert.glsl", GLSLShader::VERTEX);
        m_prog.compileShader("shaders/shadewire.frag.glsl", GLSLShader::FRAGMENT);
        m_prog.compileShader("shaders/shadewire.geom.glsl", GLSLShader::GEOMETRY);
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
