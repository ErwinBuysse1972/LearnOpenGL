#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f) 
{
    CFuncTracer trace("SceneBasic_Uniform::SceneBasic_Uniform", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_Uniform::initScene()
{
    CFuncTracer trace("SceneBasic_Uniform::initScene", m_trace);
    try
    {
        compile();

        std::cout << std::endl;

        m_prog.printActiveUniforms();

        /////////////////// Create the VBO ////////////////////
        float positionData[] = {
            -0.8f, -0.8f, 0.0f,
             0.8f, -0.8f, 0.0f,
             0.0f,  0.8f, 0.0f };
        float colorData[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f };

        // Create and populate the buffer objects
        GLuint vboHandles[2];
        glGenBuffers(2, vboHandles);
        GLuint positionBufferHandle = vboHandles[0];
        GLuint colorBufferHandle = vboHandles[1];

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

        // Create and set-up the vertex array object
        glGenVertexArrays(1, &m_vaoHandle);
        glBindVertexArray(m_vaoHandle);

        glEnableVertexAttribArray(0);  // Vertex position
        glEnableVertexAttribArray(1);  // Vertex color

        glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat) * 3);
        glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat) * 3);

        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(1, 1);
        glBindVertexArray(0);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_Uniform::compile()
{
    CFuncTracer trace("SceneBasic_Uniform::compile", m_trace);
    try 
    {
        m_prog.compileShader("shaders/basic_uniform.vert.glsl");
        m_prog.compileShader("shaders/basic_uniform.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        trace.Error("GLSL Exception occurred: %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_Uniform::update(float t)
{
    CFuncTracer trace("SceneBasic_Uniform::update", m_trace);
    static int count = 0;
    try
    {
        if (++count == 800)
        {
            count = 0;
            if (m_animate) {
                m_angle += 1.0f;
                if (m_angle >= 360.0f) m_angle -= 360.0f;
            }
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_Uniform::render()
{
    try
    {
        glClear(GL_COLOR_BUFFER_BIT);

        m_rotationMatrix = glm::rotate(glm::mat4(1.0f), m_angle, vec3(0.0f, 0.0f, 1.0f));

        GLuint programHandle = m_prog.getHandle();
        GLuint location = glGetUniformLocation(programHandle, "RotationMatrix");
        if ((int)location >= 0)
            glUniformMatrix4fv(location, 1, GL_FALSE, &m_rotationMatrix[0][0]);

        glBindVertexArray(m_vaoHandle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    catch (const std::exception& )
    {
    }

}

void SceneBasic_Uniform::resize(int w, int h)
{
    CFuncTracer trace("SceneBasic_Uniform::resize", m_trace);
    try
    {
        m_width = w;
        m_height = h;
        glViewport(0, 0, w, h);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}