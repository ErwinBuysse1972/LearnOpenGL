#include "SceneSpaceAmbientOcclusion.h"

#include <random.h>
#include <texture.h>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;

SceneSpaceAmbientOcclusion::SceneSpaceAmbientOcclusion(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_deferredFBO(0)
    , m_quad(0)
    , m_tPrev(0)
    , m_rotSpeed(1.0f)
    , m_plane(10, 10, 1, 1, 10, 7)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::SceneSpaceAmbientOcclusion", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::initScene()
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::initScene", m_trace);
    try
    {
        compileAndLinkShader();
        glEnable(GL_DEPTH_TEST);

        //m_bunny = ObjMesh::load("../media/bunny/bunny.obj");
        m_bunny = ObjMesh::load("../media/dragon.obj");
        float c = 1.5f;
        m_angle = glm::pi<float>() / 2.0f;
        m_sceneProj = glm::perspective(glm::radians(50.0f), (float)m_width / m_height, 0.3f, 100.0f);

        // Array for quad
        GLfloat verts[] = {
            -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
        };
        GLfloat tc[] = {
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
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

        m_woodTex = Texture::loadTexture("../media/texture/hardwood2_diffuse.jpg");
        m_brickTex = Texture::loadTexture("../media/texture/brick1.jpg");

        m_prog.setUniform("Light.L", glm::vec3(0.3f));
        m_prog.setUniform("Light.La", glm::vec3(0.5f));

        // Create and assign the random sample kernel
        buildKernel();

        // Create the texture of random rotation directions
        GLuint rotTex = buildRandRotationTex();
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, rotTex);

        // Setup the G-Buffers
        setupFrameBufferObjects();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::update(float t)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::update", m_trace);
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
void SceneSpaceAmbientOcclusion::render()
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::render", m_trace);
    try
    {
        pass1();  // Render to G-Buffers
        pass2();  // SSAO
        pass3();  // Blur
        pass4();  // Lighting
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::renderGuiWindow", m_trace);
    try
    {
 
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::resize(int w, int h)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::resize", m_trace);
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
GLuint SceneSpaceAmbientOcclusion::buildRandRotationTex(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::buildRandRotationTex", m_trace);
    try
    {
        int size = 4;
        std::vector<GLfloat> randDirections(3 * size * size);
        for (int i = 0; i < size * size; i++) {
            glm::vec3 v = m_rand.uniformCircle();
            randDirections[i * 3 + 0] = v.x;
            randDirections[i * 3 + 1] = v.y;
            randDirections[i * 3 + 2] = v.z;
        }

        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, size, size);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGB, GL_FLOAT, randDirections.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        return tex;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::setMatrices(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::setMatrices", m_trace);
    try
    {
        glm::mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(mv));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/ssao.vert.glsl");
        m_prog.compileShader("shaders/ssao.frag.glsl");
        m_prog.link();
        m_prog.use();
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
void SceneSpaceAmbientOcclusion::setupFrameBufferObjects(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::setupFrameBufferObjects", m_trace);
    try
    {
        GLuint depthBuf, posTex, normTex, colorTex;

        // Create the textures for position, normal and color
        createGBufTex(GL_TEXTURE0, GL_RGB32F, posTex);  // Position
        createGBufTex(GL_TEXTURE1, GL_RGB32F, normTex); // Normal
        createGBufTex(GL_TEXTURE2, GL_RGB8, colorTex);  // Color

        createGBufTex(GL_TEXTURE3, GL_R16F, m_aoTex[0]);     // AO pair
        createGBufTex(GL_TEXTURE3, GL_R16F, m_aoTex[1]);

        // Create and bind the FBO
        glGenFramebuffers(1, &m_deferredFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);

        // The depth buffer
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

        // Attach the textures to the framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

        GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_NONE };
        glDrawBuffers(5, drawBuffers);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Create and bind the FBO
        glGenFramebuffers(1, &m_ssaoFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);

        // Attach the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_aoTex[0], 0);

        GLenum drawBuffers2[] = { GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(5, drawBuffers2);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::createGBufTex(GLenum texUnit, GLenum format, GLuint& texid)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::createGBufTex", m_trace);
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
void SceneSpaceAmbientOcclusion::buildKernel(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::buildKernel", m_trace);
    try
    {
        int kernSize = 64;
        std::vector<float> kern(3 * kernSize);
        for (int i = 0; i < kernSize; i++) {
            glm::vec3 randDir = m_rand.uniformHemisphere();
            float scale = ((float)(i * i)) / (kernSize * kernSize);
            randDir *= glm::mix(0.1f, 1.0f, scale);

            kern[i * 3 + 0] = randDir.x;
            kern[i * 3 + 1] = randDir.y;
            kern[i * 3 + 2] = randDir.z;
            //std::cout << randDir.x << ", " << randDir.y << ", " << randDir.z << std::endl;
        }

        GLuint progHandle = m_prog.getHandle();
        GLint loc = glGetUniformLocation(progHandle, "SampleKernel");
        glUniform3fv(loc, kernSize, kern.data());
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::pass1(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::pass1", m_trace);
    try
    {
        m_prog.setUniform("Pass", 1);

        glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(
            glm::vec3(2.1f, 1.5f, 2.1f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        m_projection = m_sceneProj;

        drawScene();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::pass2(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::pass2", m_trace);
    try
    {
        m_prog.setUniform("Pass", 2);

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);

        // Write to aoTex[0]
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_aoTex[0], 0);

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        m_prog.setUniform("ProjectionMatrix", m_sceneProj);

        drawScreenFillingQuad();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::pass3(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::pass3", m_trace);
    try
    {
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_prog.setUniform("Pass", 3);

        // Read from aoTex[0], write to aoTex[1]
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_aoTex[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_aoTex[1], 0);

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        drawScreenFillingQuad();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::pass4(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::pass4", m_trace);
    try
    {
        m_prog.setUniform("Pass", 4);

        // Read from aoTex[1] (blurred)
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_aoTex[1]);

        // Revert to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        drawScreenFillingQuad();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::drawScreenFillingQuad(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::drawScreenFillingQuad", m_trace);
    try
    {
        m_view = glm::mat4(1.0);
        m_model = glm::mat4(1.0);
        m_projection = glm::mat4(1.0);
        setMatrices();

        // Render the quad
        glBindVertexArray(m_quad);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSpaceAmbientOcclusion::drawScene(void)
{
    CFuncTracer trace("SceneSpaceAmbientOcclusion::drawScene", m_trace);
    try
    {
        m_prog.setUniform("Light.Position", m_view * glm::vec4(3.0f, 3.0f, 1.5f, 1.0f));

        // Walls
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_woodTex);
        m_prog.setUniform("Material.UseTex", 1);
        m_model = glm::mat4(1.0f);
        setMatrices();
        m_plane.render();

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_brickTex);
        m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2));
        m_model = glm::rotate(m_model, glm::radians(90.0f), glm::vec3(1, 0, 0));
        setMatrices();
        m_plane.render();

        m_model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0));
        m_model = glm::rotate(m_model, glm::radians(90.0f), glm::vec3(0, 1, 0));
        m_model = glm::rotate(m_model, glm::radians(90.0f), glm::vec3(1, 0, 0));
        setMatrices();
        m_plane.render();

        m_prog.setUniform("Material.UseTex", 0);
        m_prog.setUniform("Material.Kd", glm::vec3(0.9f, 0.5f, 0.2f));
        m_model = glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_model = glm::scale(m_model, glm::vec3(2.0f));
        m_model = glm::translate(m_model, glm::vec3(0, 0.282958, 0));

        setMatrices();
        m_bunny->render();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
