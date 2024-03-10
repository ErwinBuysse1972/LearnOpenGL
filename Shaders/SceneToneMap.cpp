#include "SceneToneMap.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneHDRLightToneMap::SceneHDRLightToneMap(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
    , m_plane(20.0f, 10.0f, 1, 1)
    , m_teapot(14, mat4(1.0f))
    , m_sphere(2.0f,50, 50)
{
    CFuncTracer trace("SceneHDRLightToneMap::SceneHDRLightToneMap", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::initScene()
{
    CFuncTracer trace("SceneHDRLightToneMap::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

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
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
        glEnableVertexAttribArray(0);  // Vertex position

        glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
        glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
        glEnableVertexAttribArray(2);  // Texture coordinates

        glBindVertexArray(0);

        setupFrameBufferObject();

        vec3 intense = vec3(5.0f);
        m_prog.setUniform("Lights[0].L", intense);
        m_prog.setUniform("Lights[1].L", intense);
        m_prog.setUniform("Lights[2].L", intense);
        intense = vec3(0.2f);
        m_prog.setUniform("Lights[0].La", intense);
        m_prog.setUniform("Lights[1].La", intense);
        m_prog.setUniform("Lights[2].La", intense);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::update(float t)
{
    CFuncTracer trace("SceneHDRLightToneMap::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::render()
{
    CFuncTracer trace("SceneHDRLightToneMap::render", m_trace);
    try
    {
        pass1();
        computeLogAveLuminance();
        pass2();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneHDRLightToneMap::renderGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneHDRLightToneMap::renderOtherGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::resize(int w, int h)
{
    CFuncTracer trace("SceneHDRLightToneMap::resize", m_trace);
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
void SceneHDRLightToneMap::setMatrices()
{
    CFuncTracer trace("SceneHDRLightToneMap::setMatrices", m_trace);
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
void SceneHDRLightToneMap::compileAndLinkShader()
{
    CFuncTracer trace("SceneHDRLightToneMap::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/tonemap.vert.glsl");
        m_prog.compileShader("shaders/tonemap.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        trace.Error("GLSLProgram exception %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::setupFrameBufferObject()
{
    CFuncTracer trace("SceneHDRLightToneMap::setupFrameBufferObject", m_trace);
    try
    {
        GLuint depthBuf;

        // Create and bind the FBO
        glGenFramebuffers(1, &m_hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);

        // The depth buffer
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

        // The HDR color buffer
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &m_hdrTex);
        glBindTexture(GL_TEXTURE_2D, m_hdrTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_width, m_height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Attach the images to the framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdrTex, 0);

        GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(2, drawBuffers);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::pass1()
{
    CFuncTracer trace("SceneHDRLightToneMap::pass1", m_trace);
    try
    {
        m_prog.setUniform("Pass", 1);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glViewport(0, 0, m_width, m_height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(2.0f, 0.0f, 14.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = glm::perspective(glm::radians(60.0f), (float)m_width / m_height, 0.3f, 100.0f);

        drawScene();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::pass2()
{
    CFuncTracer trace("", m_trace);
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
void SceneHDRLightToneMap::computeLogAveLuminance()
{
    CFuncTracer trace("SceneHDRLightToneMap::computeLogAveLuminance", m_trace);
    try
    {
        int size = m_width * m_height;
        std::vector<GLfloat> texData(size * 3);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_hdrTex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData.data());
        float sum = 0.0f;
        for (int i = 0; i < size; i++) {
            float lum = glm::dot(vec3(texData[i * 3 + 0], texData[i * 3 + 1], texData[i * 3 + 2]),
                vec3(0.2126f, 0.7152f, 0.0722f));
            sum += logf(lum + 0.00001f);
        }
        m_prog.setUniform("AveLum", expf(sum / size));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRLightToneMap::drawScene()
{
    CFuncTracer trace("SceneHDRLightToneMap::drawScene", m_trace);
    try
    {
        vec3 intense = vec3(1.0f);
        m_prog.setUniform("Lights[0].L", intense);
        m_prog.setUniform("Lights[1].L", intense);
        m_prog.setUniform("Lights[2].L", intense);

        vec4 lightPos = vec4(0.0f, 4.0f, 2.5f, 1.0f);
        lightPos.x = -7.0f;
        m_prog.setUniform("Lights[0].Position", m_view * lightPos);
        lightPos.x = 0.0f;
        m_prog.setUniform("Lights[1].Position", m_view * lightPos);
        lightPos.x = 7.0f;
        m_prog.setUniform("Lights[2].Position", m_view * lightPos);

        m_prog.setUniform("Material.Kd", 0.9f, 0.3f, 0.2f);
        m_prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
        m_prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        m_prog.setUniform("Material.Shininess", 100.0f);

        // The backdrop plane
        m_model = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_plane.render();

        // The bottom plane
        m_model = glm::translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f));
        setMatrices();
        m_plane.render();

        // Top plane
        m_model = glm::translate(mat4(1.0f), vec3(0.0f, 5.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(180.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_plane.render();

        m_prog.setUniform("Material.Kd", vec3(0.4f, 0.9f, 0.4f));
        m_model = glm::translate(mat4(1.0f), vec3(-3.0f, -3.0f, 2.0f));
        setMatrices();
        m_sphere.render();

        m_prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.9f));
        m_model = glm::translate(mat4(1.0f), vec3(3.0f, -5.0f, 1.5f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}