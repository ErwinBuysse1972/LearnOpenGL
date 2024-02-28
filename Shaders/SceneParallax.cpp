#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include "SceneParallax.h"
#include <texture.h>

SceneParallax::SceneParallax(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_plane(8, 8, 1, 1)
    , m_tPrev(0.0f)
{
    CFuncTracer trace("SceneParallax::SceneParallax", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneParallax::initScene()
{
    CFuncTracer trace("SceneParallax::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        m_model = glm::mat4(1.0f);
        m_angle = glm::radians(90.0f);

        // Load textures
        m_normalMap = Texture::loadTexture("../media/texture/mybrick/mybrick-normal.png");
        m_heightMap = Texture::loadTexture("../media/texture/mybrick/mybrick-height.png");
        m_colorMap = Texture::loadTexture("../media/texture/mybrick/mybrick-color.png");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_colorMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_normalMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_heightMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        m_view = glm::lookAt(  glm::vec3(-1, 0, 8), // eye
                               glm::vec3(-1, 0, 0), // center
                               glm::vec3(0, 1, 0)); // up

        m_prog.use();
        m_prog.setUniform("Light.L", glm::vec3(0.7f));
        m_prog.setUniform("Light.La", glm::vec3(0.01f));
        m_prog.setUniform("Material.Ks", glm::vec3(0.7f));
        m_prog.setUniform("Material.Shininess", 40.0f);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneParallax::update(float t)
{
    CFuncTracer trace("SceneParallax::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;
        m_angle = glm::mod(m_angle + deltaT * (1.5f), glm::two_pi<float>());
        if (animating()) {}
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneParallax::render()
{
    CFuncTracer trace("SceneParallax::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_prog.setUniform("Light.Position", m_view * glm::vec4(2.0f, 2.0f, 1.0f, 1.0f));

        m_model = glm::rotate(glm::mat4(1.0f), 
                  glm::radians(65.0f), 
                  glm::vec3(0, 1, 0));
        m_model = glm::rotate(m_model, 
                              glm::radians(90.0f), 
                              glm::vec3(1, 0, 0));

        SetMatrices(m_prog);
        m_plane.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneParallax::resize(int w, int h)
{
    CFuncTracer trace("SceneParallax::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(35.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneParallax::SetMatrices(GLSLProgram& p)
{
    CFuncTracer trace("SceneParallax::SetMatrices", m_trace);
    try
    {
        glm::mat4 mv = m_view * m_model;
        p.setUniform("ModelViewMatrix", mv);
        p.setUniform("NormalMatrix", glm::mat3(mv));
        p.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneParallax::CompileAndLinkShader()
{
    CFuncTracer trace("SceneParallax::CompileAndLinkShader", m_trace);
    try
    {
//        m_prog.compileShader("shaders/parallax.vert.glsl");
//        m_prog.compileShader("shaders/parallax.frag.glsl");
//        m_prog.link();

        m_prog.compileShader("shaders/steep-parallax.vert.glsl");
        m_prog.compileShader("shaders/steep-parallax.frag.glsl");
        m_prog.link();
    }
    catch (GLSLProgramException& e) {
        std::cerr << e.what() << std::endl;
        trace.Error("GLSL program exception : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
