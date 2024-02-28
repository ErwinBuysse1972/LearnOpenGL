#include "sceneflat.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneFlat::SceneFlat(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
{
    CFuncTracer trace("SceneFlat::SceneFlat", m_trace);
    try
    {
        m_ogre = ObjMesh::load("../media/bs_ears.obj");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFlat::initScene()
{
    CFuncTracer trace("SceneFlat::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(0.0f, 0.35f, 0.85f), vec3(0.0f, -0.25f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
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
void SceneFlat::update(float t)
{
    CFuncTracer trace("SceneFlat::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFlat::render()
{
    CFuncTracer trace("SceneFlat::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::vec4 worldLight = glm::vec4(2.0f, 4.0f, 1.0f, 1.0f);
        m_model = glm::rotate(mat4(1.0f), glm::radians(m_angle), vec3(0.0f, 1.0f, 0.0f));
        m_prog.setUniform("Light.Position", m_view * m_model * worldLight);

        m_model = mat4(1.0f);
        m_model = glm::rotate(m_model, glm::radians(25.0f), vec3(0.0f, 1.0f, 0.0f));

        SetMatrices();
        m_ogre->render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneFlat::resize(int w, int h)
{
    CFuncTracer trace("SceneFlat::resize", m_trace);
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
void SceneFlat::SetMatrices(void)
{
    CFuncTracer trace("SceneFlat::SetMatrices", m_trace);
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
void SceneFlat::CompileAndLinkShader()
{
    CFuncTracer trace("SceneFlat::CompileAndLinkShader", m_trace);
    try 
    {
        m_prog.compileShader("shaders/flat.vert.glsl");
        m_prog.compileShader("shaders/flat.frag.glsl");
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
