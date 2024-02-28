#include <iostream>
using std::cerr;
using std::endl;
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include <texture.h>
#include "SceneReflectCube.h"

SceneReflectCube::SceneReflectCube(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_teapot(14, mat4(1.0f))
    , m_sky(100.0f)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
{
    CFuncTracer trace("SceneReflectCube::SceneReflectCube", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneReflectCube::initScene()
{
    CFuncTracer trace("SceneReflectCube::initScene", m_trace);
    try
    {
        CompileAndLinkShader();
        glEnable(GL_DEPTH_TEST);
        m_projection = mat4(1.0f);
        m_angle = glm::radians(90.0f);

        GLuint cubeTex = Texture::loadHdrCubeMap("../media/texture/cube/pisa-hdr/pisa");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneReflectCube::update(float t)
{
    CFuncTracer trace("SceneReflectCube::update", m_trace);
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
        exit(EXIT_FAILURE);
    }
}
void SceneReflectCube::render()
{
    CFuncTracer trace("SceneReflectCube::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos = vec3(7.0f * cos(m_angle), 2.0f, 7.0f * sin(m_angle));
        m_view = glm::lookAt(   cameraPos, 
                                vec3(0.0f, 0.0f, 0.0f), 
                                vec3(0.0f, 1.0f, 0.0f));

        // Draw sky
        m_skyProg.use();
        m_model = mat4(1.0f);
        SetMatrices(m_skyProg);
        m_sky.render();

        // Draw scene
        m_prog.use();
        m_prog.setUniform("WorldCameraPosition", cameraPos);
        m_prog.setUniform("MaterialColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
        m_prog.setUniform("ReflectFactor", 0.85f);

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, -1.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        SetMatrices(m_prog);
        m_teapot.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneReflectCube::resize(int w, int h)
{
    CFuncTracer trace("SceneReflectCube::resize", m_trace);
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
void SceneReflectCube::SetMatrices(GLSLProgram& p) 
{
    CFuncTracer trace("SceneReflectCube::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        p.setUniform("ModelMatrix", m_model);
        p.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneReflectCube::CompileAndLinkShader() 
{
    CFuncTracer trace("", m_trace);
    try
    {
        m_prog.compileShader("shaders/cubemap_reflects.vert.glsl");
        m_prog.compileShader("shaders/cubemap_reflects.frag.glsl");

        m_skyProg.compileShader("shaders/skybox.vert.glsl");
        m_skyProg.compileShader("shaders/skybox.frag.glsl");

        m_skyProg.link();

        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

