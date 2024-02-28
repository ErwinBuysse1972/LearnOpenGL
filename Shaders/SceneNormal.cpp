#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::mat4;
using glm::vec3;
#include "texture.h"
#include "SceneNormal.h"


SceneNormalMap::SceneNormalMap(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
{
    CFuncTracer trace("SceneNormalMap::SceneNormalMap", m_trace);
    try
    {
        m_ogre = ObjMesh::load("../media/bs_ears.obj", false, true);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneNormalMap::initScene()
{
    CFuncTracer trace("SceneNormalMap::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt( vec3(-1.0f, 0.25f, 2.0f),     //eye
                              vec3(0.0f, 0.0f, 0.0f),       //center
                              vec3(0.0f, 1.0f, 0.0f));      //up
        m_projection = mat4(1.0f);

        m_angle = glm::radians(100.0f);

        m_prog.setUniform("Light.L", vec3(1.0f));
        m_prog.setUniform("Light.La", vec3(0.2f));

        // Load diffuse texture
        GLuint diffTex = Texture::loadTexture("../media/texture/ogre_diffuse.png");

        // Load normal map
        GLuint normalTex = Texture::loadTexture("../media/texture/ogre_normalmap.png");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffTex);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalTex);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneNormalMap::update(float t)
{
    CFuncTracer trace("SceneNormalMap::update", m_trace);
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
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneNormalMap::render() 
{
    CFuncTracer trace("SceneNormalMap::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_prog.setUniform("Light.Position", m_view * glm::vec4(10.0f * cos(m_angle), 1.0f, 10.0f * sin(m_angle), 1.0f));
        m_prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
        m_prog.setUniform("Material.Shininess", 1.0f);

        m_model = mat4(1.0f);
        SetMatrices();
        m_ogre->render();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneNormalMap::resize(int w, int h)
{
    CFuncTracer trace("SceneNormalMap::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        float c = 2.0f;
        m_projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneNormalMap::SetMatrices()
{
    CFuncTracer trace("SceneNormalMap::SetMatrices", m_trace);
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
void SceneNormalMap::CompileAndLinkShader()
{
    CFuncTracer trace("SceneNormalMap::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/normalmap.vert.glsl");
        m_prog.compileShader("shaders/normalmap.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}

