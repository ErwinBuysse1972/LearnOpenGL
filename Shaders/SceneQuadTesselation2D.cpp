#include "SceneQuadTesselation2D.h"

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;

SceneQuadTesselation2D::SceneQuadTesselation2D(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_inner(4)
    , m_outer(4)
    , m_lineWidth(1.5f)
{
    CFuncTracer trace("SceneQuadTesselation2D::SceneQuadTesselation2D", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::initScene()
{
    CFuncTracer trace("SceneQuadTesselation2D::initScene", m_trace);
    try
    {
        compileAndLinkShader();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        float c = 3.5f;
        m_projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);


        // Set up patch VBO
        float v[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };

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

        GLint maxVerts;
        glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxVerts);
        printf("Max patch vertices: %d\n", maxVerts);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::update(float t)
{
    CFuncTracer trace("SceneQuadTesselation2D::update", m_trace);
    try
    {
        ///////////// Uniforms ////////////////////
        m_prog.setUniform("Inner", m_inner);
        m_prog.setUniform("Outer", m_outer);
        m_prog.setUniform("LineWidth", m_lineWidth);
        m_prog.setUniform("LineColor", vec4(0.05f, 0.0f, 0.05f, 1.0f));
        m_prog.setUniform("QuadColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
        /////////////////////////////////////////////

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::render()
{
    CFuncTracer trace("SceneQuadTesselation2D::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec3 cameraPos(0.0f, 0.0f, 1.5f);
        m_view = glm::lookAt(cameraPos,
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f));

        m_model = mat4(1.0f);
        setMatrices();

        glBindVertexArray(m_vaoHandle);
        glDrawArrays(GL_PATCHES, 0, 4);

        glFinish();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneQuadTesselation2D::renderGuiWindow", m_trace);
    try
    {
        ImGui::Begin("Tesselation quad 2D parameters");
        ImGui::SliderInt("inner", &m_inner, 1, 50);
        ImGui::SliderInt("outer", &m_outer, 1, 50);
        ImGui::SliderFloat("Line width", &m_lineWidth, 0.1f, 3.0f);
        ImGui::End();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneQuadTesselation2D::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::resize(int w, int h)
{
    CFuncTracer trace("SceneQuadTesselation2D::resize", m_trace);
    try
    {

        float w2 = w / 2.0f;
        float h2 = h / 2.0f;
        m_viewport = mat4(  vec4(w2, 0.0f, 0.0f, 0.0f),
                            vec4(0.0f, h2, 0.0f, 0.0f),
                            vec4(0.0f, 0.0f, 1.0f, 0.0f),
                            vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
        m_width = w;
        m_height = h;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::setMatrices(void)
{
    CFuncTracer trace("SceneQuadTesselation2D::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("MVP", m_projection * mv);
        m_prog.setUniform("ViewportMatrix", m_viewport);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneQuadTesselation2D::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneQuadTesselation2D::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/quadtess.vert.glsl");
        m_prog.compileShader("shaders/quadtess.frag.glsl");
        m_prog.compileShader("shaders/quadtess.geom.glsl");
        m_prog.compileShader("shaders/quadtess.tes.glsl");
        m_prog.compileShader("shaders/quadtess.tcs.glsl");
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
