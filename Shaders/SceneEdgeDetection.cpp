#include "SceneEdgeDetection.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include <cfunctracer.h>

SceneEdgeDetection::SceneEdgeDetection(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
    , m_plane(50.0f, 50.0f, 1, 1)
    , m_teapot(14, mat4(1.0f))
    , m_torus(0.7f * 1.5f, 0.3f * 1.5f, 50, 50)
{
    CFuncTracer trace("SceneEdgeDetection::SceneEdgeDetection", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::initScene()
{
    CFuncTracer trace("SceneEdgeDetection::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        m_projection = mat4(1.0f);
        m_angle = glm::pi<float>() / 4.0f;

        setupFrameBufferObjects();

        // Array for full-screen quad
        GLfloat verts[] = {
            -1.0f, -1.0f, 0.0f, 
             1.0f, -1.0f, 0.0f, 
             1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 
             1.0f,  1.0f, 0.0f, 
            -1.0f,  1.0f, 0.0f
        };
        GLfloat tc[] = {
            0.0f, 0.0f, 1.0f, 
            0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 
            1.0f, 0.0f, 1.0f
        };

        // Set up the buffers

        unsigned int handle[2];
        glGenBuffers(2, handle);

        glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

        // Set up the vertex array object

        glGenVertexArrays(1, &m_fsQuad);
        glBindVertexArray(m_fsQuad);

        glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);  // Vertex position

        glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
        glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);  // Texture coordinates

        glBindVertexArray(0);

        m_prog.setUniform("EdgeThreshold", 0.05f);
        m_prog.setUniform("Light.L", vec3(1.0f));
        m_prog.setUniform("Light.La", vec3(0.2f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::update(float t)
{
    CFuncTracer trace("SceneEdgeDetection::update", m_trace);
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
void SceneEdgeDetection::render()
{
    CFuncTracer trace("SceneEdgeDetection::render", m_trace);
    try
    {
        pass1();
        glFlush();
        pass2();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::resize(int w, int h)
{
    CFuncTracer trace("SceneEdgeDetection::resize", m_trace);
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
    }
}
void SceneEdgeDetection::setMatrices()
{
    CFuncTracer trace("SceneEdgeDetection::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::compileAndLinkShader()
{
    CFuncTracer trace("SceneEdgeDetection::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/edge.vert.glsl");
        m_prog.compileShader("shaders/edge.frag.glsl");
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
    }
}
void SceneEdgeDetection::setupFrameBufferObjects(void)
{
    CFuncTracer trace("SceneEdgeDetection::setupFrameBufferObjects", m_trace);
    try
    {
        // Generate and bind the framebuffer
        glGenFramebuffers(1, &m_fboHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

        // Create the texture object
        glGenTextures(1, &m_renderTex);
        glBindTexture(GL_TEXTURE_2D, m_renderTex);

        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_width, m_height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        // Bind the texture to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTex, 0);

        // Create the depth buffer
        GLuint depthBuf;
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

        // Bind the depth buffer to the FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

        // Set the targets for the fragment output variables
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);

        // Unbind the framebuffer, and revert to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::pass1(void)
{
    CFuncTracer trace("SceneEdgeDetection::pass1", m_trace);
    try
    {
        m_prog.setUniform("Pass", 1);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_view = glm::lookAt(vec3(7.0f * cos(m_angle), 4.0f, 7.0f * sin(m_angle))
                           , vec3(0.0f, 0.0f, 0.0f)
                           , vec3(0.0f, 1.0f, 0.0f));
        m_projection = glm::perspective(glm::radians(60.0f), (float)m_width / m_height, 0.3f, 100.0f);

        m_prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
        m_prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        m_prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
        m_prog.setUniform("Material.Shininess", 100.0f);

        m_model = mat4(1.0f);
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();

        m_prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
        m_prog.setUniform("Material.Shininess", 1.0f);
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, -0.75f, 0.0f));
        setMatrices();
        m_plane.render();

        m_prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.2f);
        m_prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        m_prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
        m_prog.setUniform("Material.Shininess", 100.0f);
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(1.0f, 1.0f, 3.0f));
        m_model = glm::rotate(m_model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_torus.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneEdgeDetection::pass2(void)
{
    CFuncTracer trace("SceneEdgeDetection::pass2", m_trace);
    try
    {
        m_prog.setUniform("Pass", 2);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_renderTex);

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        m_model = mat4(1.0f);
        m_view = mat4(1.0f);
        m_projection = mat4(1.0f);
        setMatrices();

        // Render the full-screen quad
        glBindVertexArray(m_fsQuad);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
