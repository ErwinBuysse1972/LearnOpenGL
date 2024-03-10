#include "SceneRenderToTex.h"
#include <texture.h>
#include <memory>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;


SceneRenderToTex::SceneRenderToTex(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_cow_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
{
    CFuncTracer trace("SceneRenderToTex::SceneRenderToTex", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRenderToTex::initScene()
{
    CFuncTracer trace("SceneRenderToTex::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        GLuint spotTexture = Texture::loadTexture("../media/spot/spot_texture.png");
        m_spot = ObjMesh::load("../media/spot/spot_triangulated.obj");

        m_projection = mat4(1.0f);
        m_angle = glm::radians(140.0f);
        m_prog.setUniform("Light.L", vec3(1.0));
        m_prog.setUniform("Light.La", vec3(0.15f));

        setupFrameBufferObjects();

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, spotTexture);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRenderToTex::update(float t)
{
    CFuncTracer trace("SceneRenderToTex::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0) deltaT = 0;
        m_tPrev = t;

        m_angle += m_rotSpeed * deltaT;
        m_cow_angle += m_rotSpeed * 2 * deltaT;
        if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
        if (m_cow_angle > glm::two_pi<float>()) m_cow_angle -= glm::two_pi<float>();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRenderToTex::render()
{
    CFuncTracer trace("SceneRenderToTex::render", m_trace);
    try
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
        renderToTexture();
        glFlush();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        renderScene();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRenderToTex::renderGuiWindow(ImGuiIO* io)
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
void SceneRenderToTex::renderOtherGuiWindow(ImGuiIO* io)
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
void SceneRenderToTex::resize(int w, int h)
{
    CFuncTracer trace("SceneRenderToTex::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}


void SceneRenderToTex::setMatrices(void)
{
    CFuncTracer trace("SceneRenderToTex::setMatrices", m_trace);
    try
    {
        mat4 mv = m_model * m_view;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRenderToTex::compileAndLinkShader(void)
{
    CFuncTracer trace("", m_trace);
    try
    {
        m_prog.compileShader("shaders/rendertotex.vert.glsl");
        m_prog.compileShader("shaders/rendertotex.frag.glsl");
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
void SceneRenderToTex::setupFrameBufferObjects(void)
{
    CFuncTracer trace("SceneRenderToTex::setupFrameBufferObjects", m_trace);
    try
    {
        // Generate and bind the framebuffer
        glGenFramebuffers(1, &m_fboHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

        // Create the texture object
        GLuint renderTex;
        glGenTextures(1, &renderTex);
        glActiveTexture(GL_TEXTURE0);  // Use texture unit 0
        glBindTexture(GL_TEXTURE_2D, renderTex);
#ifdef __APPLE__
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#else
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
#endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Bind the texture to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

        // Create the depth buffer
        GLuint depthBuf;
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

        // Bind the depth buffer to the FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, depthBuf);

        // Set the targets for the fragment output variables
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);

        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result == GL_FRAMEBUFFER_COMPLETE) 
        {
            cout << "Framebuffer is complete" << endl;
        }
        else 
        {
            cout << "Framebuffer error: " << result << endl;
        }

        // Unbind the framebuffer, and revert to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRenderToTex::renderToTexture(void)
{
    CFuncTracer trace("SceneRenderToTex::renderToTexture", m_trace);
    try
    {
        m_prog.setUniform("RenderTex", 1);
        glViewport(0, 0, 512, 512);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        m_view = glm::lookAt(vec3(0.0f, 0.0f, 2.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        vec3 cameraPos = vec3(2.0f * cos(m_cow_angle), 1.5f, 2.0f * sin(m_cow_angle));
        m_view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

        m_projection = glm::perspective(glm::radians(50.0f), 1.0f, 0.3f, 100.0f);

        m_prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        m_prog.setUniform("Material.Shininess", 100.0f);

        m_model = mat4(0.5f);
        //m_model = glm::rotate(m_model, m_angle, vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_spot->render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneRenderToTex::renderScene(void)
{
    CFuncTracer trace("SceneRenderToTex::renderScene", m_trace);
    try
    {
        m_prog.setUniform("RenderTex", 0);
        glViewport(0, 0, m_width, m_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos = vec3(2.0f * cos(m_angle), 1.5f, 2.0f * sin(m_angle));
        m_view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

        m_projection = glm::perspective(glm::radians(45.0f), (float)m_width / m_height, 0.3f, 100.0f);

        m_prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Shininess", 1.0f);

        m_model = mat4(1.0f);
        setMatrices();
        m_cube.render();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

