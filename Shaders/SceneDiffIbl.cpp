#include "SceneDiffIbl.h"
#include <texture.h>

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb/stb_image.h"

SceneDiffIbl::SceneDiffIbl(std::shared_ptr<CTracer> tracer)
    :Scene(tracer)
    , m_tPrev(0.0f)
    , m_camAngle(glm::half_pi<float>())
    , m_rotSpeed(0.5f)
    , m_camPos(0,4,7)
{
    CFuncTracer trace("SceneDiffIbl::SceneDiffIbl", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDiffIbl::initScene()
{
    CFuncTracer trace("SceneDiffIbl::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        m_spot = ObjMesh::load("../media/spot/spot_triangulated.obj");

        glEnable(GL_DEPTH_TEST);

        m_model = glm::mat4(1.0f);

        //stbi_set_flip_vertically_on_load(true);
        m_diffCube = Texture::loadHdrCubeMap("../media/texture/cube/grace-diffuse/grace-diffuse");
        //stbi_set_flip_vertically_on_load(false);
        m_cube = Texture::loadHdrCubeMap("../media/texture/cube/grace/grace");
        m_colorTex = Texture::loadTexture("../media/spot/spot_texture.png");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDiffIbl::update(float t)
{
    CFuncTracer trace("SceneDiffIbl::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;


        if (animating()) {
            m_camAngle = glm::mod(m_camAngle + deltaT * m_rotSpeed, glm::two_pi<float>());
            m_camPos.x = glm::cos(m_camAngle) * 4.0f;
            m_camPos.y = 0.0f;
            m_camPos.z = glm::sin(m_camAngle) * 4.0f;
        }
        m_view = glm::lookAt(   m_camPos,
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDiffIbl::render()
{
    CFuncTracer trace("SceneDiffIbl::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_model = glm::mat4(1.0f);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cube);
        m_sbProg.use();
        setMatrices(m_sbProg);
        m_skyBox.render();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_diffCube);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_colorTex);
        m_prog.use();
        m_prog.setUniform("CamPos", m_camPos);
        m_prog.setUniform("Material.Color", glm::vec3(0.4f, 0.4f, 0.4f));
        m_model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
        setMatrices(m_prog);
        m_spot->render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDiffIbl::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneDiffIbl::renderGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDiffIbl::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneDiffIbl::renderOtherGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDiffIbl::resize(int w, int h)
{
    CFuncTracer trace("SceneDiffIbl::resize", m_trace);
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
    }
}
void SceneDiffIbl::setMatrices(GLSLProgram& p)
{
    CFuncTracer trace("SceneDiffIbl::setMatrices", m_trace);
    try
    {
        glm::mat4 mv = m_view * m_model;
        p.setUniform("ModelMatrix", m_model);
        p.setUniform("ModelViewMatrix", mv);
        p.setUniform("NormalMatrix", glm::mat3(mv));
        p.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDiffIbl::compileAndLinkShader(void)
{
    CFuncTracer trace("", m_trace);
    try
    {
        m_prog.compileShader("shaders/diffuseibl.vert.glsl");
        m_prog.compileShader("shaders/diffuseibl.frag.glsl");
        m_prog.link();

        m_sbProg.compileShader("shaders/skybox.vert.glsl");
        m_sbProg.compileShader("shaders/skybox.frag.glsl");
        m_sbProg.link();
    }
    catch (GLSLProgramException& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
