#include "SceneShadowMap.h"
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
#include <memory>

SceneShadowMap::SceneShadowMap(std::shared_ptr<CTracer> tracer)
    :Scene(tracer)
    , m_tPrev(0)
    , m_shadowMapWidth(512)
    , m_shadowMapHeight(512)
    , m_teapot(14, glm::mat4(1.0f))
    , m_plane(40.0f, 40.0f, 2, 2)
    , m_torus(0.7f * 2.0f, 0.3f * 2.0f, 50, 50)
{
    CFuncTracer trace("SceneShadowMap::SceneShadowMap", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::initScene()
{
    CFuncTracer trace("SceneShadowMap::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        m_angle = glm::quarter_pi<float>();

        // Set up the framebuffer object
        setupFrameBufferObjects();

        GLuint programHandle = m_prog.getHandle();
        m_pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "recordDepth");
        m_pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");

        m_shadowBias = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
            vec4(0.0f, 0.5f, 0.0f, 0.0f),
            vec4(0.0f, 0.0f, 0.5f, 0.0f),
            vec4(0.5f, 0.5f, 0.5f, 1.0f)
        );

        float c = 1.65f;
        vec3 lightPos = vec3(0.0f, c * 5.25f, c * 7.5f);  // World coords
        m_lightFrustum.orient(lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_lightFrustum.setPerspective(50.0f, 1.0f, 1.0f, 25.0f);
        m_lightPV = m_shadowBias * m_lightFrustum.getProjectionMatrix() * m_lightFrustum.getViewMatrix();

        m_prog.setUniform("Light.Intensity", vec3(0.85f));
        m_prog.setUniform("ShadowMap", 0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::update(float t)
{
    CFuncTracer trace("SceneShadowMap::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;

        m_angle += 0.2f * deltaT;
        if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::render()
{
    CFuncTracer trace("SceneShadowMap::render", m_trace);
    try
    {
        m_prog.use();
        // Pass 1 (shadow map generation)
        m_view = m_lightFrustum.getViewMatrix();
        m_projection = m_lightFrustum.getProjectionMatrix();
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, m_shadowMapWidth, m_shadowMapHeight);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_pass1Index);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.5f, 10.0f);
        drawScene();
        glCullFace(GL_BACK);
        glFlush();
        //spitOutDepthBuffer(); // This is just used to get an image of the depth buffer

        // Pass 2 (render)
        float c = 2.0f;
        vec3 cameraPos(c * 11.5f * cos(m_angle), c * 7.0f, c * 11.5f * sin(m_angle));
        m_view = glm::lookAt(cameraPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_prog.setUniform("Light.Position", m_view * vec4(m_lightFrustum.getOrigin(), 1.0f));
        m_projection = glm::perspective(glm::radians(50.0f), (float)m_width / m_height, 0.1f, 100.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, m_width, m_height);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_pass2Index);
        drawScene();

        // Draw the light's frustum
        m_solidProg.use();
        m_solidProg.setUniform("Color", vec4(1.0f, 0.0f, 0.0f, 1.0f));
        mat4 mv = m_view * m_lightFrustum.getInverseViewMatrix();
        m_solidProg.setUniform("MVP", m_projection * mv);
        m_lightFrustum.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneShadowMap::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneShadowMap::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::resize(int w, int h)
{
    CFuncTracer trace("SceneShadowMap::resize", m_trace);
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
void SceneShadowMap::setMatrices(void)
{
    CFuncTracer trace("SceneShadowMap::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
        m_prog.setUniform("ShadowMatrix", m_lightPV * m_model);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneShadowMap::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/shadowmap.vert.glsl");
        m_prog.compileShader("shaders/shadowmap.frag.glsl");
        m_prog.link();
        m_prog.use();

        // Used when rendering light frustum
        m_solidProg.compileShader("shaders/solid.vert.glsl", GLSLShader::VERTEX);
        m_solidProg.compileShader("shaders/solid.frag.glsl", GLSLShader::FRAGMENT);
        m_solidProg.link();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSLProgram exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::setupFrameBufferObjects(void)
{
    CFuncTracer trace("SceneShadowMap::setupFrameBufferObjects(", m_trace);
    try
    {
        GLfloat border[] = { 1.0f, 0.0f,0.0f,0.0f };
        // The depth buffer texture
        GLuint depthTex;
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, m_shadowMapWidth, m_shadowMapHeight);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

        // Assign the depth buffer texture to texture channel 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTex);

        // Create and set up the FBO
        glGenFramebuffers(1, &m_shadowFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, depthTex, 0);

        GLenum drawBuffers[] = { GL_NONE };
        glDrawBuffers(1, drawBuffers);

        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result == GL_FRAMEBUFFER_COMPLETE) {
            printf("Framebuffer is complete.\n");
        }
        else {
            printf("Framebuffer is not complete.\n");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::drawScene(void)
{
    CFuncTracer trace("SceneShadowMap::drawScene", m_trace);
    try
    {
        vec3 color = vec3(0.7f, 0.5f, 0.3f);
        m_prog.setUniform("Material.Ka", color * 0.05f);
        m_prog.setUniform("Material.Kd", color);
        m_prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
        m_prog.setUniform("Material.Shininess", 150.0f);
        m_model = mat4(1.0f);
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();

        m_prog.setUniform("Material.Ka", color * 0.05f);
        m_prog.setUniform("Material.Kd", color);
        m_prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
        m_prog.setUniform("Material.Shininess", 150.0f);
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, 2.0f, 5.0f));
        m_model = glm::rotate(m_model, glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_torus.render();

        m_prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
        m_prog.setUniform("Material.Shininess", 1.0f);
        m_model = mat4(1.0f);
        setMatrices();
        m_plane.render();
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(-5.0f, 5.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
        setMatrices();
        m_plane.render();
        m_model = mat4(1.0f);
        m_model = glm::translate(m_model, vec3(0.0f, 5.0f, -5.0f));
        m_model = glm::rotate(m_model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_plane.render();
        m_model = mat4(1.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneShadowMap::splitOutDepthBuffer(void)
{
    CFuncTracer trace("SceneShadowMap::splitOutDepthBuffer", m_trace);
    try
    {
        int size = m_shadowMapWidth * m_shadowMapHeight;
        float* buffer = new float[size];
        unsigned char* imgBuffer = new unsigned char[size * 4];

        glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, buffer);

        for (int i = 0; i < m_shadowMapHeight; i++)
            for (int j = 0; j < m_shadowMapWidth; j++)
            {
                int imgIdx = 4 * ((i * m_shadowMapWidth) + j);
                int bufIdx = ((m_shadowMapHeight - i - 1) * m_shadowMapWidth) + j;

                // This is just to make a more visible image.  Scale so that
                // the range (minVal, 1.0) maps to (0.0, 1.0).  This probably should
                // be tweaked for different light configurations.
                float minVal = 0.88f;
                float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
                unsigned char val = (unsigned char)(scale * 255);
                imgBuffer[imgIdx] = val;
                imgBuffer[imgIdx + 1] = val;
                imgBuffer[imgIdx + 2] = val;
                imgBuffer[imgIdx + 3] = 0xff;
            }

        delete[] buffer;
        delete[] imgBuffer;
        exit(1);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
