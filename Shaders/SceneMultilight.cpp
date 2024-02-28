#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include "SceneMultilight.h"

SceneMultiLight::SceneMultiLight(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_plane(10.0f, 10.0f, 100, 100)
{
    CFuncTracer trace("SceneMultiLight::SceneMultiLight", m_trace);
    try
    {
        m_mesh = ObjMesh::load("../media/pig_triangulated.obj", true);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception ocucrred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiLight::initScene()
{
    CFuncTracer trace("SceneMultiLight::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

       m_view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
       m_projection = mat4(1.0f);

        float x, z;
        for (int i = 0; i < 5; i++) {
            std::stringstream name;
            name << "lights[" << i << "].Position";
            x = 2.0f * cosf((glm::two_pi<float>() / 5) * i);
            z = 2.0f * sinf((glm::two_pi<float>() / 5) * i);
            m_prog.setUniform(name.str().c_str(), m_view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
        }

        m_prog.setUniform("lights[0].L", vec3(0.0f, 0.8f, 0.8f));
        m_prog.setUniform("lights[1].L", vec3(0.0f, 0.0f, 0.8f));
        m_prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));
        m_prog.setUniform("lights[3].L", vec3(0.0f, 0.8f, 0.0f));
        m_prog.setUniform("lights[4].L", vec3(0.8f, 0.8f, 0.8f));

        m_prog.setUniform("lights[0].La", vec3(0.0f, 0.2f, 0.2f));
        m_prog.setUniform("lights[1].La", vec3(0.0f, 0.0f, 0.2f));
        m_prog.setUniform("lights[2].La", vec3(0.2f, 0.0f, 0.0f));
        m_prog.setUniform("lights[3].La", vec3(0.0f, 0.2f, 0.0f));
        m_prog.setUniform("lights[4].La", vec3(0.2f, 0.2f, 0.2f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception ocucrred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiLight::update(float t)
{
    CFuncTracer trace("SceneMultiLight::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception ocucrred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiLight::render()
{
    CFuncTracer trace("SceneMultiLight::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        m_prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        m_prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        m_prog.setUniform("Material.Shininess", 180.0f);

        m_model = mat4(1.0f);
        m_model = glm::rotate(m_model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
        SetMatrices();
        m_mesh->render();

        m_prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
        m_prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        m_prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
        m_prog.setUniform("Material.Shininess", 180.0f);
        
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, -0.45f, 0.0f));
        SetMatrices();
        m_plane.render();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception ocucrred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiLight::resize(int w, int h)
{
    CFuncTracer trace("SceneMultiLight::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception ocucrred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiLight::SetMatrices()
{
    CFuncTracer trace("SceneMultiLight::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception ocucrred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMultiLight::CompileAndLinkShader()
{
    CFuncTracer trace("SceneMultiLight::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/multilight.vert.glsl");
        m_prog.compileShader("shaders/multilight.frag.glsl");
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
        trace.Error("Exception ocucrred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

