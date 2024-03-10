#include "SceneSilhouette.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneSilhouette::SceneSilhouette(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
{
    CFuncTracer trace("SceneSilhouette::SceneSilhouette", m_trace);
    try
    {
        m_ogre = ObjMesh::loadWithAdjacency("../media/bs_ears.obj");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneSilhouette::initScene()
{
    CFuncTracer trace("SceneSilhouette::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        m_angle = glm::half_pi<float>();

        ///////////// Uniforms ////////////////////
        m_prog.setUniform("EdgeWidth", 0.015f);
        m_prog.setUniform("PctExtend", 0.25f);
        m_prog.setUniform("LineColor", vec4(0.05f, 0.0f, 0.05f, 1.0f));
        
        m_prog.setUniform("Material.Kd", 0.7f, 0.5f, 0.2f);
        m_prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        
        m_prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);
        /////////////////////////////////////////////
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneSilhouette::update(float t)
{
    CFuncTracer trace("SceneSilhouette::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;

        m_angle += m_rotSpeed * deltaT;
        if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneSilhouette::render()
{
    CFuncTracer trace("SceneSilhouette::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos(1.5f * cos(m_angle), 0.0f, 1.5f * sin(m_angle));
        m_view = glm::lookAt(   cameraPos,
                                vec3(0.0f, -0.2f, 0.0f),
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
void SceneSilhouette::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneSilhouette::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneSilhouette::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneSilhouette::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneSilhouette::resize(int w, int h)
{
    CFuncTracer trace("SceneSilhouette::SceneSilhouette", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        float c = 1.5f;
        m_projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
        m_height = h;
        m_width = w;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneSilhouette::setMatrices(void)
{
    CFuncTracer trace("SceneSilhouette::SceneSilhouette", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneSilhouette::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneSilhouette::SceneSilhouette", m_trace);
    try
    {
        m_prog.compileShader("shaders/silhouette.vert.glsl", GLSLShader::VERTEX);
        m_prog.compileShader("shaders/silhouette.frag.glsl", GLSLShader::FRAGMENT);
        m_prog.compileShader("shaders/silhouette.geom.glsl", GLSLShader::GEOMETRY);
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
