#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include "texture.h"
#include "SceneMultiTex.h"

SceneMultiTex::SceneMultiTex(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_cube()
{
    CFuncTracer trace("SceneMultiTex::SceneMultiTex", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiTex::initScene()
{
    CFuncTracer trace("SceneMultiTex::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(1.0f, 1.25f, 1.25f),  // eye
                             vec3(0.0f, 0.0f, 0.0f),    // center
                             vec3(0.0f, 1.0f, 0.0f));   // up
        m_projection = mat4(1.0f);

        m_prog.setUniform("Light.L", vec3(1.0f));
        m_prog.setUniform("Light.La", vec3(0.15f));

        m_brick = Texture::loadTexture("../media/texture/brick1.jpg");
        m_moss = Texture::loadTexture("../media/texture/moss.png");

        // Load brick texture file into channel 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_brick);
        // Load moss texture file into channel 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_moss);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiTex::update(float t)
{
    CFuncTracer trace("SceneMultiTex::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiTex::render()
{
    CFuncTracer trace("SceneMultiTex::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Ks", 0.05f, 0.05f, 0.05f);
        m_prog.setUniform("Material.Shininess", 1.0f);

        m_model = mat4(1.0f);
        SetMatrices();
        m_cube.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiTex::resize(int w, int h)
{
    CFuncTracer trace("SceneMultiTex::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiTex::SetMatrices()
{
    CFuncTracer trace("SceneMultiTex::SetMatrices", m_trace);
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
        exit(EXIT_FAILURE);
    }
}
void SceneMultiTex::CompileAndLinkShader()
{
    CFuncTracer trace("SceneMultiTex::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/multitex.vert.glsl");
        m_prog.compileShader("shaders/multitex.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        trace.Error("GLSL Program exception : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
