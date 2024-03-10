#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include <texture.h>
#include "SceneRefractCube.h"


SceneRefractCube::SceneRefractCube(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_teapot(14.0f, mat4(1.0f))
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
{
    CFuncTracer trace("SceneRefractCube::SceneRefractCube", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneRefractCube::initScene()
{
    CFuncTracer trace("SceneRefractCube::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);
        m_angle = glm::radians(90.0f);

        GLuint cubeTex = Texture::loadHdrCubeMap("../media/texture/cube/pisa-hdr/pisa");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneRefractCube::update(float t)
{
    CFuncTracer trace("SceneRefractCube::update", m_trace);
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
        exit(EXIT_FAILURE);
    }
}
void SceneRefractCube::render()
{
    CFuncTracer trace("SceneRefractCube::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos = vec3(7.0f * cos(m_angle), 2.0f, 7.0f * sin(m_angle));
        m_view = glm::lookAt(cameraPos, 
                             vec3(0.0f, 0.0f, 0.0f), 
                             vec3(0.0f, 1.0f, 0.0f));

        // Draw sky
        m_skyProg.use();
        m_model = mat4(1.0f);
        SetMatrices(m_skyProg);
        m_sky.render();

        // Draw teapot
        m_prog.use();
        m_prog.setUniform("WorldCameraPosition", cameraPos);
        m_prog.setUniform("Material.Eta", 0.94f);
        m_prog.setUniform("Material.ReflectionFactor", 0.1f);

        m_model = glm::translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        SetMatrices(m_prog);
        m_teapot.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneRefractCube::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneRefractCube::renderGuiWindow", m_trace);
    try
    {
        static float f = 0.0f;
        static int counter = 0;


        ImGui::Begin("Hello world!");

        ImGui::Text("This is some usefull text");
        ImGui::Checkbox("Demo Window", &m_show_demo_window);
        ImGui::Checkbox("Another window", &m_show_other_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&m_clearColor);

        if (ImGui::Button("Button"))
            m_ulCountClicks++;

        ImGui::SameLine();
        ImGui::Text("Counter clicks = %ld", m_ulCountClicks);
        ImGui::Text("Application average %.3f ms/frame (%.2f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRefractCube::resize(int w, int h)
{
    CFuncTracer trace("SceneRefractCube::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(50.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneRefractCube::SetMatrices(GLSLProgram& p)
{
    CFuncTracer trace("SceneRefractCube::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        p.setUniform("ModelMatrix", m_model);
        p.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneRefractCube::CompileAndLinkShader()
{
    CFuncTracer trace("SceneRefractCube::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/cubemap_refract.vert.glsl");
        m_prog.compileShader("shaders/cubemap_refract.frag.glsl");

        m_skyProg.compileShader("shaders/skybox.vert.glsl");
        m_skyProg.compileShader("shaders/skybox.frag.glsl");
        m_skyProg.link();
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL Program exception occurred : %s", e.what());
        exit(EXIT_FAILURE);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
