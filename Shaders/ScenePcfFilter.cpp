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

#include "ScenePcfFilter.h"

ScenePcfFilter::ScenePcfFilter(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_plane(20.0f, 20.0f, 1, 1)
    , m_shadowMapWidth(512)
    , m_shadowMapHeight(512)
    , m_tPrev(0.0f)
{
    CFuncTracer trace("ScenePcfFilter::ScenePcfFilter", m_trace);
    try
    {
        m_mesh = ObjMesh::load("../media/building.obj");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::initScene()
{
    CFuncTracer trace("ScenePcfFilter::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        m_angle = glm::two_pi<float>() * 0.85f;

        // Set up the framebuffer object
        setupFrameBufferObjects();

        GLuint programHandle = m_prog.getHandle();
        m_pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "recordDepth");
        m_pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");

        m_shadowScale = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
            vec4(0.0f, 0.5f, 0.0f, 0.0f),
            vec4(0.0f, 0.0f, 0.5f, 0.0f),
            vec4(0.5f, 0.5f, 0.5f, 1.0f)
        );

        m_lightPos = vec3(-2.5f, 2.0f, -2.5f);  // World coords
        m_lightFrustum.orient(m_lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_lightFrustum.setPerspective(40.0f, 1.0f, 0.1f, 100.0f);

        m_lightPV = m_shadowScale * m_lightFrustum.getProjectionMatrix() * m_lightFrustum.getViewMatrix();

        m_prog.setUniform("Light.Intensity", vec3(0.85f));
        m_prog.setUniform("ShadowMap", 0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::update(float t)
{
    CFuncTracer trace("ScenePcfFilter::update", m_trace);
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
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::render()
{
    CFuncTracer trace("ScenePcfFilter::render", m_trace);
    try
    {
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
        drawBuildingScene();
        glCullFace(GL_BACK);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glFlush();

        // Pass 2 (render)
        vec3 cameraPos(1.8f * cos(m_angle), 0.7f, 1.8f * sin(m_angle));
        m_view = glm::lookAt(cameraPos, vec3(0.0f, -0.175f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

        m_prog.setUniform("Light.Position", m_view * vec4(m_lightFrustum.getOrigin(), 1.0));
        m_projection = glm::perspective(glm::radians(50.0f), (float)m_width / m_height, 0.1f, 100.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, m_width, m_height);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_pass2Index);
        drawBuildingScene();
        glFinish();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("ScenePcfFilter::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("ScenePcfFilter::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::resize(int w, int h)
{
    CFuncTracer trace("ScenePcfFilter::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::setMatrices(void)
{
    CFuncTracer trace("ScenePcfFilter::setMatrices", m_trace);
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
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::compileAndLinkShader(void)
{
    CFuncTracer trace("ScenePcfFilter::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/pcf.vert.glsl");
        m_prog.compileShader("shaders/pcf.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::setupFrameBufferObjects(void)
{
    CFuncTracer trace("ScenePcfFilter::setupFrameBufferObjects", m_trace);
    try
    {
        GLfloat border[] = { 1.0f,0.0f,0.0f,0.0f };
        // The depth buffer texture
        GLuint depthTex;
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, m_shadowMapWidth, m_shadowMapHeight);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

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
        trace.Error("Exception occured : %s", ex.what());
    }
}
void ScenePcfFilter::drawBuildingScene(void)
{
    CFuncTracer trace("ScenePcfFilter::drawBuildingScene", m_trace);
    try
    {
        vec3 color = vec3(1.0f, 0.85f, 0.55f);
        m_prog.setUniform("Material.Ka", color * 0.1f);
        m_prog.setUniform("Material.Kd", color);
        m_prog.setUniform("Material.Ks", vec3(0.0f));
        m_prog.setUniform("Material.Shininess", 1.0f);
        m_model = mat4(1.0f);
        setMatrices();
        m_mesh->render();

        m_prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
        m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
        m_prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
        m_prog.setUniform("Material.Shininess", 1.0f);
        m_model = mat4(1.0f);
        setMatrices();
        m_plane.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
