#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;

#include "texture.h"

#include "scenealphatest.h"


SceneAlphaTest::SceneAlphaTest(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_teapot(14, mat4(1.0f))
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 3.0f)
{
    CFuncTracer trace("SceneAlphaTest::SceneAlphaTest", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneAlphaTest::initScene()
{
    CFuncTracer trace("SceneAlphaTest::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_projection = mat4(1.0f);

        m_prog.setUniform("Light.L", vec3(1.0f));
        m_prog.setUniform("Light.La", vec3(0.2f));

        // Load texture files
        GLuint cement = Texture::loadTexture("../media/texture/cement.jpg");
        GLuint moss = Texture::loadTexture("../media/texture/moss.png");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cement);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, moss);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

void SceneAlphaTest::update(float t)
{
    CFuncTracer trace("SceneAlphaTest::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;

        if (this->m_animate) {
            m_angle += m_rotSpeed * deltaT;
            if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

void SceneAlphaTest::render()
{
    CFuncTracer trace("", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos = vec3(6.0f * cos(m_angle), 0.25f, 6.0f * sin(m_angle));
        m_view = glm::lookAt(cameraPos, 
                             vec3(0.0f, 0.0f, 0.0f), 
                             vec3(0.0f, 1.0f, 0.0f));

        m_prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Shininess", 1.0f);

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, -1.5f, 0.0f));
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

void SceneAlphaTest::resize(int w, int h)
{
    CFuncTracer trace("SceneAlphaTest::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

void SceneAlphaTest::SetMatrices()
{
    CFuncTracer trace("SceneAlphaTest::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

void SceneAlphaTest::CompileAndLinkShader()
{
    CFuncTracer trace("SceneAlphaTest::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/alphatest.vert.glsl");
        m_prog.compileShader("shaders/alphatest.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception :%s", e.what());
        exit(EXIT_FAILURE);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

