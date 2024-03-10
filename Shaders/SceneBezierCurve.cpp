#include "SceneBezierCurve.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;


SceneBezierCurve::SceneBezierCurve(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_numSegments(50)
    , m_numStrips(1)
{
    CFuncTracer trace("SceneBezierCurve::SceneBezierCurve", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::initScene()
{
    CFuncTracer trace("SceneBezierCurve::initScene", m_trace);
    try
    {
        compileAndLinkShader();
        glEnable(GL_DEPTH_TEST);

        float c = 3.5f;
        m_projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
        glPointSize(10.0f);

        // Set up patch VBO
        float v[] = { -1.0f, -1.0f, -0.5f, 1.0f, 0.5f, -1.0f, 1.0f, 1.0f };

        GLuint vboHandle;
        glGenBuffers(1, &vboHandle);

        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), v, GL_STATIC_DRAW);

        // Set up patch VAO
        glGenVertexArrays(1, &m_vaoHandle);
        glBindVertexArray(m_vaoHandle);

        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Set the number of vertices per patch.  IMPORTANT!!
        glPatchParameteri(GL_PATCH_VERTICES, 4);

        m_solidProg.use();
        m_solidProg.setUniform("Color", vec4(0.5f, 1.0f, 1.0f, 1.0f));

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::update(float t)
{
    CFuncTracer trace("SceneBezierCurve::update", m_trace, false);
    try
    {
        // Segments and strips may be inverted on NVIDIA
        m_prog.use();
        m_prog.setUniform("NumSegments", m_numSegments);
        m_prog.setUniform("NumStrips", m_numStrips);
        m_prog.setUniform("LineColor", vec4(1.0f, 1.0f, 0.5f, 1.0f));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::render()
{
    CFuncTracer trace("SceneBezierCurve::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos(0.0f, 0.0f, 1.5f);
        m_view = glm::lookAt(   cameraPos,
                                vec3(0.0f, 0.0f, 0.0f),
                                vec3(0.0f, 1.0f, 0.0f));
        m_model = mat4(1.0f);

        glBindVertexArray(m_vaoHandle);
        setMatrices();

        // Draw the curve
        m_prog.use();
        glDrawArrays(GL_PATCHES, 0, 4);

        // Draw the control points
        m_solidProg.use();
        glDrawArrays(GL_POINTS, 0, 4);

        glFinish();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneBezierCurve::renderGuiWindow", m_trace, false);
    try
    {
        ImGui::Begin("Bezier parameters");
        ImGui::SliderInt("Segments", &m_numSegments, 5, 100);
        ImGui::SliderInt("Strips", &m_numStrips, 1, 20);
        ImGui::End();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneBezierCurve::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::resize(int w, int h)
{
    CFuncTracer trace("SceneBezierCurve::resize", m_trace);
    try
    {
        m_height = h;
        m_width = w;
        glViewport(0, 0, w, h);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::setMatrices(void)
{
    CFuncTracer trace("SceneBezierCurve::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.use();
        m_prog.setUniform("MVP", m_projection * mv);
        m_solidProg.use();
        m_solidProg.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}
void SceneBezierCurve::compileAndLinkShader()
{
    CFuncTracer trace("SceneBezierCurve::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/bezcurve.vert.glsl");
        m_prog.compileShader("shaders/bezcurve.frag.glsl");
        m_prog.compileShader("shaders/bezcurve.tes.glsl");
        m_prog.compileShader("shaders/bezcurve.tcs.glsl");
        m_prog.link();
        m_prog.use();

        m_solidProg.compileShader("shaders/solid.vert.glsl");
        m_solidProg.compileShader("shaders/solid.frag.glsl");
        m_solidProg.link();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured: %s", ex.what());
    }
}

