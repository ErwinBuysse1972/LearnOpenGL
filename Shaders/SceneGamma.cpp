
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include <cfuncTracer.h>
#include "SceneGamma.h"

SceneGamma::SceneGamma(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
    , m_plane(50.0f, 50.0f, 1, 1)
    , m_torus(0.7f * 1.5f, 0.3f * 1.5f, 50, 50)
{
    CFuncTracer trace("SceneGamma::SceneGamma", m_trace);
    try
    {
        m_ogre = ObjMesh::load("../media/bs_ears.obj");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::initScene()
{
    CFuncTracer trace("SceneGamma::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        float c = 2.5f;
        m_projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
        m_angle = glm::pi<float>() / 2.0f;

        m_prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
        m_prog.setUniform("Gamma", 2.2f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::update(float t)
{
    CFuncTracer trace("SceneGamma::update", m_trace);
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
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::render()
{
    CFuncTracer trace("SceneGamma::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_view = glm::lookAt(vec3(3.0f * cos(m_angle), 0.0f, 3.0f * sin(m_angle)), // camera position
                             vec3(0.0f, 0.0f, 0.0f), // target
                            vec3(0.0f, 1.0f, 0.0f)); //up

        m_prog.setUniform("Light.Position", m_view * glm::vec4(10.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Ka", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Shininess", 100.0f);

        m_model = mat4(1.0f);
        setMatrices();
        m_ogre->render();

        glFinish();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneGamma::renderGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneGamma::renderOtherGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::resize(int w, int h)
{
    CFuncTracer trace("SceneGamma::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::setMatrices(void)
{
    CFuncTracer trace("SceneGamma::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneGamma::compileAndLinkShader(void)
{
    CFuncTracer trace("", m_trace);
    try
    {
        m_prog.compileShader("shaders/gamma.vert.glsl");
        m_prog.compileShader("shaders/gamma.frag.glsl");
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
    }
}

