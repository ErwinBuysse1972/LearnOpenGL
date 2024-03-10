#include "ScenePointSprites.h"
#include <texture.h>

#include <cstdlib>
using std::rand;
using std::srand;

#include <ctime>
using std::time;

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;


ScenePointSprites::ScenePointSprites(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
{
    CFuncTracer trace("ScenePointSprites::ScenePointSprites", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::initScene()
{
    CFuncTracer trace("ScenePointSprites::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        m_numSprites = 50;
        m_locations = new float[m_numSprites * 3];
        srand((unsigned int)time(0));
        for (int i = 0; i < m_numSprites; i++) 
        {
            vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
                ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
                ((float)rand() / RAND_MAX * 2.0f) - 1.0f);
            m_locations[i * 3] = p.x;
            m_locations[i * 3 + 1] = p.y;
            m_locations[i * 3 + 2] = p.z;
        }

        // Set up the buffers
        GLuint handle;
        glGenBuffers(1, &handle);

        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, m_numSprites * 3 * sizeof(float), m_locations, GL_STATIC_DRAW);

        delete[] m_locations;

        // Set up the vertex array object
        glGenVertexArrays(1, &m_sprites);
        glBindVertexArray(m_sprites);

        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
        glEnableVertexAttribArray(0);  // Vertex position

        glBindVertexArray(0);

        // Load texture file
        const char* texName = "../media/texture/flower.png";
        Texture::loadTexture(texName);

        m_prog.setUniform("SpriteTex", 0);
        m_prog.setUniform("Size2", 0.15f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::update(float t)
{
    CFuncTracer trace("ScenePointSprites::update", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::render()
{
    CFuncTracer trace("ScenePointSprites::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos(0.0f, 0.0f, 3.0f);
        m_view = glm::lookAt(   cameraPos,
                                vec3(0.0f, 0.0f, 0.0f),
                                vec3(0.0f, 1.0f, 0.0f));

        m_model = mat4(1.0f);
        setMatrices();

        glBindVertexArray(m_sprites);
        glDrawArrays(GL_POINTS, 0, m_numSprites);

        glFinish();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("ScenePointSprites::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("ScenePointSprites::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::resize(int w, int h)
{
    CFuncTracer trace("ScenePointSprites::resize", m_trace);
    try
    {
        m_height = h;
        m_width = w;
        glViewport(0, 0, w, h);
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::setMatrices(void)
{
    CFuncTracer trace("ScenePointSprites::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("ProjectionMatrix", m_projection);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePointSprites::compileAndLinkShader(void)
{
    CFuncTracer trace("", m_trace);
    try
    {
        m_prog.compileShader("shaders/pointssprite.vert.glsl");
        m_prog.compileShader("shaders/pointssprite.frag.glsl");
        m_prog.compileShader("shaders/pointssprite.geom.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
