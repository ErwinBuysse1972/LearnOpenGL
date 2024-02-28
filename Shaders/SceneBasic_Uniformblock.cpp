#include "scenebasic_uniformblock.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
using std::string;

#include <iostream>
using std::endl;
using std::cerr;

#include "glutils.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;

SceneBasic_UniformBlock::SceneBasic_UniformBlock(std::shared_ptr<CTracer> tracer) 
    : Scene(tracer)
    , m_angle(0.0f) 
{ 
    CFuncTracer trace("SceneBasic_UniformBlock::SceneBasic_UniformBlock", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_UniformBlock::initUniformBlockBuffer()
{
    CFuncTracer trace("SceneBasic_UniformBlock::initUniformBlockBuffer", m_trace);
    try
    {
        GLuint programHandle = m_prog.getHandle();

        // Get the index of the uniform block
        GLuint blockIndex = glGetUniformBlockIndex(programHandle, "BlobSettings");

        // Allocate space for the buffer
        GLint blockSize;
        glGetActiveUniformBlockiv(programHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        GLubyte* blockBuffer;
        blockBuffer = (GLubyte*)malloc(blockSize);

        // Query for the offsets of each block variable
        const GLchar* names[] = { "BlobSettings.InnerColor",
                                    "BlobSettings.OuterColor",
                                    "BlobSettings.RadiusInner",
                                    "BlobSettings.RadiusOuter" };

        GLuint indices[4];
        glGetUniformIndices(programHandle, 4, names, indices);

        GLint offset[4];
        glGetActiveUniformsiv(programHandle, 4, indices, GL_UNIFORM_OFFSET, offset);

        // Store data within the buffer at the appropriate offsets
        GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };
        GLfloat innerRadius = 0.25f;
        GLfloat outerRadius = 0.45f;

        memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
        memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
        memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
        memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

        // Create the buffer object and copy the data
        GLuint uboHandle;
        glGenBuffers(1, &uboHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
        glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

        // Bind the buffer object to the uniform block
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboHandle);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_UniformBlock::initScene()
{
    CFuncTracer trace("SceneBasic_UniformBlock::initScene", m_trace);
    try
    {
        compile();

        std::cout << std::endl;
        initUniformBlockBuffer();
        m_prog.printActiveUniformBlocks();

        /////////////////// Create the VBO ////////////////////
        float positionData[] = {
            -0.8f, -0.8f, 0.0f,
             0.8f, -0.8f, 0.0f,
             0.8f,  0.8f, 0.0f,
             -0.8f, -0.8f, 0.0f,
              0.8f, 0.8f, 0.0f,
              -0.8f, 0.8f, 0.0f
        };
        float tcData[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };

        // Create and populate the buffer objects
        GLuint vboHandles[2];
        glGenBuffers(2, vboHandles);
        GLuint positionBufferHandle = vboHandles[0];
        GLuint tcBufferHandle = vboHandles[1];

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tcData, GL_STATIC_DRAW);

        // Create and set-up the vertex array object
        glGenVertexArrays(1, &m_vaoHandle);
        glBindVertexArray(m_vaoHandle);

        glEnableVertexAttribArray(0);  // Vertex position
        glEnableVertexAttribArray(1);  // Vertex texture coords

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

        glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_UniformBlock::compile()
{
    CFuncTracer trace("SceneBasic_UniformBlock::compile", m_trace);
    try {
        m_prog.compileShader("shaders/basic_uniformblock.vert.glsl");
        m_prog.compileShader("shaders/basic_uniformblock.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        trace.Error("GLSL Exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneBasic_UniformBlock::update(float t)
{
    m_angle += 1.0f;
    if (m_angle >= 360.0f) m_angle -= 360.0f;
}

void SceneBasic_UniformBlock::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneBasic_UniformBlock::resize(int w, int h)
{
    CFuncTracer trace("SceneBasic_UniformBlock::resize", m_trace);
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