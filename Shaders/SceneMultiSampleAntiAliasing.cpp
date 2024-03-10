#include "SceneMultiSampleAntiAliasing.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneMultiSampleAntiAliasing::SceneMultiSampleAntiAliasing(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_rotSpeed(glm::pi<float>() / 8.0f)
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::SceneMultiSampleAntiAliasing", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMultiSampleAntiAliasing::initScene()
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        float c = 5.0f;
        m_projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
        m_angle = glm::pi<float>() / 2.0f;

        GLint bufs, samples;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
        glGetIntegerv(GL_SAMPLES, &samples);
        printf("MSAA: buffers = %d samples = %d\n", bufs, samples);
        glEnable(GL_MULTISAMPLE);

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
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMultiSampleAntiAliasing::update(float t)
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::update", m_trace);
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
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMultiSampleAntiAliasing::render()
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_view = glm::lookAt(vec3(3.0f * cos(m_angle), 0.0f, 3.0f * sin(m_angle)), 
                             vec3(0.0f, 0.0f, 0.0f), 
                             vec3(0.0f, 1.0f, 0.0f));

        m_model = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), vec3(0.0f, 0.0f, 1.0f));
        setMatrices();

        // Render the quad
        glBindVertexArray(m_quad);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glFinish();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMultiSampleAntiAliasing::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMultiSampleAntiAliasing::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMultiSampleAntiAliasing::resize(int w, int h)
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::resize", m_trace);
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
void SceneMultiSampleAntiAliasing::setMatrices(void)
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMultiSampleAntiAliasing::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneMultiSampleAntiAliasing::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/centroids.vert.glsl");
        m_prog.compileShader("shaders/centroids.frag.glsl");
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


