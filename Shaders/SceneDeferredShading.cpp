#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include "SceneDeferredShading.h"
#include <CFuncTracer.h>
#include <memory>

SceneDeferredShading::SceneDeferredShading(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
    , m_plane(50.0f, 50.0f, 1, 1)
    , m_torus(0.7f * 1.5f, 0.3f * 1.5f, 50, 50)
    , m_teapot(14, mat4(1.0))
{
    CFuncTracer trace("SceneDeferredShading::SceneDeferredShading", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::initScene()
{
    CFuncTracer trace("SceneDeferredShading::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        float c = 1.5f;
        m_angle = glm::pi<float>() / 2.0f;

        // Array for quad
        GLfloat verts[] = {
            -1.0f, -1.0f, 0.0f, 
             1.0f, -1.0f, 0.0f, 
             1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 
             1.0f,  1.0f, 0.0f, 
            -1.0f,  1.0f, 0.0f
        };
        GLfloat tc[] = {
            0.0f, 0.0f, 
            1.0f, 0.0f, 
            1.0f, 1.0f,
            0.0f, 0.0f, 
            1.0f, 1.0f, 
            0.0f, 1.0f
        };

        // Set up the buffers
        unsigned int handle[2];
        glGenBuffers(2, handle);

        glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

        // Set up the vertex array object
        glGenVertexArrays(1, &m_quad);
        glBindVertexArray(m_quad);

        glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);  // Vertex position

        glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
        glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);  // Texture coordinates

        glBindVertexArray(0);

        setupFrameBufferObjects();
        m_prog.setUniform("Light.L", vec3(1.0f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::update(float t)
{
    CFuncTracer trace("SceneDeferredShading::update", m_trace);
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
    }
}
void SceneDeferredShading::render()
{
    CFuncTracer trace("SceneDeferredShading::render", m_trace);
    try
    {
        pass1();
        pass2();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneDeferredShading::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneDeferredShading::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::resize(int w, int h)
{
    CFuncTracer trace("SceneDeferredShading::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::setMatrices(void)
{
    CFuncTracer trace("SceneDeferredShading::setMatrices", m_trace);
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
    }
}
void SceneDeferredShading::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneDeferredShading::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/deferred.vert.glsl");
        m_prog.compileShader("shaders/deferred.frag.glsl");
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
    }
}
void SceneDeferredShading::setupFrameBufferObjects(void)
{
    CFuncTracer trace("SceneDeferredShading::setupFrameBufferObjects", m_trace);
    try
    {
        GLuint depthBuf, posTex, normTex, colorTex;

        // Create and bind the FBO
        glGenFramebuffers(1, &m_deferredFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);

        // The depth buffer
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

        // Create the textures for position, normal and color
        createGBufTex(GL_TEXTURE0, GL_RGB32F, posTex);  // Position
        createGBufTex(GL_TEXTURE1, GL_RGB32F, normTex); // Normal
        createGBufTex(GL_TEXTURE2, GL_RGB8, colorTex);  // Color

        // Attach the textures to the framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

        GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                            GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(4, drawBuffers);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::pass1(void)
{
    CFuncTracer trace("SceneDeferredShading::pass1", m_trace);
    try
    {
        m_prog.setUniform("Pass", 1);

        glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(7.0f * cos(m_angle), 4.0f, 7.0f * sin(m_angle)), 
                             vec3(0.0f, 0.0f, 0.0f), 
                             vec3(0.0f, 1.0f, 0.0f));
        m_projection = glm::perspective(glm::radians(60.0f), (float)m_width / m_height, 0.3f, 100.0f);

        m_prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);

        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, 0.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();

        m_prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, -0.75f, 0.0f));
        setMatrices();
        m_plane.render();

        m_prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.2f);
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(1.0f, 1.0f, 3.0f));
        m_model = glm::rotate(m_model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_torus.render();

        glFinish();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::pass2(void)
{
    CFuncTracer trace("SceneDeferredShading::pass2", m_trace);
    try
    {
        m_prog.setUniform("Pass", 2);

        // Revert to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        m_view = mat4(1.0);
        m_model = mat4(1.0);
        m_projection = mat4(1.0);
        setMatrices();

        // Render the quad
        glBindVertexArray(m_quad);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneDeferredShading::createGBufTex(GLenum texUnit, GLenum format, GLuint& texid)
{
    CFuncTracer trace("SceneDeferredShading::createGBuffer", m_trace);
    try
    {
        glActiveTexture(texUnit);
        glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);
        glTexStorage2D(GL_TEXTURE_2D, 1, format, m_width, m_height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
