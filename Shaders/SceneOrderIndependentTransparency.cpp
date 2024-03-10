#include "SceneOrderIndependentTransparency.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include <cfunctracer.h>
#include <memory>

SceneOrderIndependentTransparency::SceneOrderIndependentTransparency(std::shared_ptr<CTracer> tracer)
    :Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
    , m_sphere(1.0f, 40, 40)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::SceneOrderIndependentTransparency", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::initScene()
{
    CFuncTracer trace("SceneOrderIndependentTransparency::initScene", m_trace);
    try
    {
        compileAndLinkShader();
        animate(false);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        m_angle = glm::radians(210.0f);

        //  glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initShaderStorage();

        GLuint programHandle = m_prog.getHandle();
        m_pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "pass1");
        m_pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "pass2");

        // Set up a  VAO for the full-screen quad
        GLfloat verts[] = { 
            -1.0f, -1.0f, 0.0f, 1.0f, 
            -1.0f,  0.0f, 1.0f, 1.0f, 
             0.0f, -1.0f, 1.0f, 0.0f };
        GLuint bufHandle;
        glGenBuffers(1, &bufHandle);
        glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

        // Set up the vertex array object
        glGenVertexArrays(1, &m_fsQuad);
        glBindVertexArray(m_fsQuad);

        glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);  // Vertex position

        glBindVertexArray(0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::update(float t)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;
        if (animating()) 
        {
            m_angle += deltaT * m_rotSpeed;
            if (m_angle > 360.0) m_angle -= 360.0f;
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::render()
{
    CFuncTracer trace("SceneOrderIndependentTransparency::render", m_trace);
    try
    {
        clearBuffers();
        pass1();
        glFlush();
        pass2();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::resize(int w, int h)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::resize", m_trace);
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
void SceneOrderIndependentTransparency::setMatrices(void)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/oit.vert.glsl");
        m_prog.compileShader("shaders/oit.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::drawScene(void)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::drawScene", m_trace);
    try
    {
        m_prog.setUniform("LightPosition", vec4(0, 0, 0, 1));
        m_prog.setUniform("LightIntensity", vec3(0.9f));

        m_prog.setUniform("Kd", vec4(0.2f, 0.2f, 0.9f, 0.55f));
        float size = 0.45f;
        for (int i = 0; i <= 6; i++)
            for (int j = 0; j <= 6; j++)
                for (int k = 0; k <= 6; k++) {
                    if ((i + j + k) % 2 == 0) {
                        m_model = glm::translate(mat4(1.0f), vec3(i - 3, j - 3, k - 3));
                        m_model = glm::scale(m_model, vec3(size));
                        setMatrices();
                        m_cube.render();
                    }
                }

        m_prog.setUniform("Kd", vec4(0.9f, 0.2f, 0.2f, 0.4f));
        size = 2.0f;
        float pos = 1.75f;
        m_model = glm::translate(mat4(1.0f), vec3(-pos, -pos, pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
        m_model = glm::translate(mat4(1.0f), vec3(-pos, -pos, -pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
        m_model = glm::translate(mat4(1.0f), vec3(-pos, pos, pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
        m_model = glm::translate(mat4(1.0f), vec3(-pos, pos, -pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
        m_model = glm::translate(mat4(1.0f), vec3(pos, pos, pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
        m_model = glm::translate(mat4(1.0f), vec3(pos, pos, -pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
        m_model = glm::translate(mat4(1.0f), vec3(pos, -pos, pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
        m_model = glm::translate(mat4(1.0f), vec3(pos, -pos, -pos));
        m_model = glm::scale(m_model, vec3(size));
        setMatrices();
        m_cube.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::initShaderStorage(void)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::initShaderStorage", m_trace);
    try
    {
        glGenBuffers(2, m_buffers);
        GLuint maxNodes = 20 * m_width * m_height;
        GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint); // The size of a linked list node

        // Our atomic counter
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, m_buffers[COUNTER_BUFFER]);
        glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

        // The buffer for the head pointers, as an image texture
        glGenTextures(1, &m_headPtrTex);
        glBindTexture(GL_TEXTURE_2D, m_headPtrTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, m_width, m_height);
        glBindImageTexture(0, m_headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

        // The buffer of linked lists
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_buffers[LINKED_LIST_BUFFER]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes * nodeSize, NULL, GL_DYNAMIC_DRAW);

        m_prog.setUniform("MaxNodes", maxNodes);

        vector<GLuint> headPtrClearBuf(m_width * m_height, 0xffffffff);
        glGenBuffers(1, &m_clearBuf);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_clearBuf);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint),
            &headPtrClearBuf[0], GL_STATIC_COPY);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::pass1(void)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::pass1", m_trace);
    try
    {
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_pass1Index);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_view = glm::lookAt(   vec3(11.0f * cos(m_angle), 2.0f, 11.0f * sin(m_angle)), 
                                vec3(0.0f, 0.0f, 0.0f), 
                                vec3(0.0f, 1.0f, 0.0f));

        m_projection = glm::perspective(glm::radians(50.0f), (float)m_width / m_height, 1.0f, 1000.0f);
        glDepthMask(GL_FALSE);
        // draw scene
        drawScene();
        glFinish();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::pass2(void)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::pass2", m_trace);
    try
    {
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_pass2Index);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_view = glm::mat4(1.0f);
        m_projection = glm::mat4(1.0f);
        m_model = glm::mat4(1.0f);

        setMatrices();

        // Draw a screen filler
        glBindVertexArray(m_fsQuad);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneOrderIndependentTransparency::clearBuffers(void)
{
    CFuncTracer trace("SceneOrderIndependentTransparency::clearBuffers", m_trace);
    try
    {
        GLuint zero = 0;
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, m_buffers[COUNTER_BUFFER]);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_clearBuf);
        glBindTexture(GL_TEXTURE_2D, m_headPtrTex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
