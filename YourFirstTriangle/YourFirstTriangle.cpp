//-----------------------------------------------------------------------------
// HelloTriangle_1.cpp by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// - Move OGL initialization code to initOpenGL() to help clean up main()
// - Moved GLFWwindow* pWindow to global variable, gWindow
// - Add glViewport() to initOpenGL()
// - Adds framebuffer resizing, glfwSetFramebufferSizeCallback() callback
// - Moved glClearColor() to initOpenGL()
// - Adds Rendering a triangle using OpenGL 3.3
// - Create const string vertex and fragment shader source code
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "Helper/cfunctracer.h"


// Global Variables
bool gWireFrame = false;
const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Triangle";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gWindow = NULL;
std::shared_ptr<CTracer> m_trace = nullptr;


// Shaders
const GLchar* vertexShaderIndexedSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;" // location 0 -> attribute index 0
"void main()"
"{"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"  //gl_Postion is a known variable name in openGL -> type is vec4 (position, w component is not used in this example)
"}";
const GLchar* fragmentShaderIndexedSrc =
"#version 330 core\n"
"out vec4 frag_color;"  // output of a fragment shader is a color w-component is alpha (RGBa)
"void main()"
"{"
"   frag_color = vec4(0.80f, 0.15f, 0.16f, 1.0f);"
"}";


const GLchar* vertexShaderInterleavedSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;" // location 0 -> attribute index 0
"layout (location = 1) in vec3 col;"
"out vec3 vert_color;"
"void main()"
"{"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"  //gl_Postion is a known variable name in openGL -> type is vec4 (position, w component is not used in this example)
"   vert_color = col;"
"}";
const GLchar* fragmentShaderInterleavedSrc =
"#version 330 core\n"
"in vec3 vert_color;"
"out vec4 frag_color;"  // output of a fragment shader is a color w-component is alpha (RGBa)
"void main()"
"{"
"   frag_color = vec4(vert_color, 1.0f);"
"}";

const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;" // location 0 -> attribute index 0
"void main()"
"{"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"  //gl_Postion is a known variable name in openGL -> type is vec4 (position, w component is not used in this example)
"}";
const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 frag_color;"  // output of a fragment shader is a color w-component is alpha (RGBa)
"void main()"
"{"
"   frag_color = vec4(0.80f, 0.15f, 0.16f, 1.0f);"
"}";

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main_YourFirstTriangle(void)
{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            trace.Error("GLFW initialization failed");
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // Set up our quad

        // 1. Set up an array of vertices for a triangle
        //   (What is a vertex?)
        GLfloat vertices[] = {
             0.0f,  0.8f, 0.0f,	// Top
             0.3f, -0.8f, 0.0f,	// Right 
            -0.5f, -0.5f, 0.0f,	// Left
        };

        // 2. Set up buffers on the GPU (vbo = vertex buffer object (identifier), vao = vertex array object (identifier)
        GLuint vbo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU (GL_STATIC_DRAW: create one/use it a lot, GL_DYNAMIC_DRAW: create it a lot/use it a lot, GL_STREAM_DRAW :create it once / use it once)

        // The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
        // variables to vertex shaders. in our case - use our only VBO, and say 'every three floats is a variable'
        // Modern OGL requires that we use a vertex array object
        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one

        // Position attribute, "0"
        glVertexAttribPointer(
            0,							// Attribute index, "0" (can have multiple attributes).  The vertex shader should have layout "0" for the position of the vertex
            3,							// Number of components of attribute "0". In this case 3 floats for x,y,z
            GL_FLOAT,					// The data type of each component
            GL_FALSE,					// Normalize component values to [-1, 1]? No, not for floating point component types (TRUE: the GPU willl normalize, FALSE: no normalization should be done)
            12,							// Stride, number of bytes between two instances of the attribute in the buffer. This buffer is "Tightly packed", the distance between two elements in this case 3 floats = 12 bytes
            NULL);						// Offset inside the structure to find the attribute

        glEnableVertexAttribArray(0);	// Enable the first attribute or attribute "0" (at index 0)

        // 3. Create vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShaderSrc, NULL);
        glCompileShader(vs);

        // Check for compile errors
        GLint result;
        GLchar infoLog[512];
        glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Vertex shader failed to compile. %s", infoLog);
            std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
        }

        // 4. Create fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
        glCompileShader(fs);

        // Check for compile errors
        glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Fragment shader failed to compile. %s", infoLog);
            std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
        }

        // 5. Create shader program and link shaders to program
        GLint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        // Check for linker errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR ) Shader program linker failure. %s", infoLog);
            std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
        }

        // Clean up shaders, do not need them anymore since they are linked to a shader program
        glDeleteShader(vs);
        glDeleteShader(fs);


        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Render the triangle
            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);


            // Swap front and back buffers
            glfwSwapBuffers(gWindow);
        }

        // Clean up
        glDeleteProgram(shaderProgram);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();

        return 0;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return -1;
}
int main_YourFirstTriangleInterleavedColor(void)
{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            trace.Error("GLFW initialization failed");
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // Set up our quad

        // 1. Set up an array of vertices for a triangle
        //   (What is a vertex?)
        GLfloat verticesInterleaved[] = {
             // position            Color (RGBà
             0.0f,  0.8f, 0.0f,	    1.0f, 0.0f, 0.0f, // Top
             0.3f, -0.8f, 0.0f,	    0.0f, 1.0f, 0.0f, // Right 
            -0.5f, -0.5f, 0.0f,	    0.0f, 0.0f, 1.0f // Left
        };

        // 2. Set up buffers on the GPU (vbo = vertex buffer object (identifier), vao = vertex array object (identifier)
        GLuint vbo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesInterleaved), verticesInterleaved, GL_STATIC_DRAW);	// copy the data from CPU to GPU (GL_STATIC_DRAW: create one/use it a lot, GL_DYNAMIC_DRAW: create it a lot/use it a lot, GL_STREAM_DRAW :create it once / use it once)

        // The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
        // variables to vertex shaders. in our case - use our only VBO, and say 'every three floats is a variable'
        // Modern OGL requires that we use a vertex array object
        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one

        // Position attribute, "0"
        glVertexAttribPointer(
            0,							// Attribute index, "0" (can have multiple attributes).  The vertex shader should have layout "0" for the position of the vertex
            3,							// Number of components of attribute "0". In this case 3 floats for x,y,z
            GL_FLOAT,					// The data type of each component
            GL_FALSE,					// Normalize component values to [-1, 1]? No, not for floating point component types (TRUE: the GPU willl normalize, FALSE: no normalization should be done)
            sizeof(GLfloat) * 6,							// Stride, number of bytes between two instances of the attribute in the buffer. This buffer is "Tightly packed", the distance between two elements in this case 3 floats = 12 bytes
            NULL);						// Offset inside the structure to find the attribute

        glEnableVertexAttribArray(0);	// Enable the first attribute or attribute "0" (at index 0)
        // Color attribute "1"
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(GLfloat) * 6,
            (GLvoid*)(sizeof(GLfloat) * 3));
        glEnableVertexAttribArray(1);

        // 3. Create vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShaderInterleavedSrc, NULL);
        glCompileShader(vs);

        // Check for compile errors
        GLint result;
        GLchar infoLog[512];
        glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Vertex shader failed to compile. %s", infoLog);
            std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
        }

        // 4. Create fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShaderInterleavedSrc, NULL);
        glCompileShader(fs);

        // Check for compile errors
        glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Fragment shader failed to compile. %s", infoLog);
            std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
        }

        // 5. Create shader program and link shaders to program
        GLint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        // Check for linker errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR ) Shader program linker failure. %s", infoLog);
            std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
        }

        // Clean up shaders, do not need them anymore since they are linked to a shader program
        glDeleteShader(vs);
        glDeleteShader(fs);


        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Render the triangle
            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);


            // Swap front and back buffers
            glfwSwapBuffers(gWindow);
        }

        // Clean up
        glDeleteProgram(shaderProgram);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();

        return 0;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return -1;
}
int main_YourFirstTriangle2Buffers(void)
{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            trace.Error("GLFW initialization failed");
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // Set up our quad

        // 1. Set up an array of vertices for a triangle
        //   (What is a vertex?)
        GLfloat vert_pos[] = {
             0.0f,  0.8f, 0.0f,	// Top
             0.3f, -0.8f, 0.0f,	// Right 
            -0.5f, -0.5f, 0.0f,	// Left
        };

        GLfloat vert_color[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };

        // 2. Set up buffers on the GPU (vbo = vertex buffer object (identifier), vao = vertex array object (identifier)
        GLuint vbo, vbo2, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vert_pos), vert_pos, GL_STATIC_DRAW);	// copy the data from CPU to GPU (GL_STATIC_DRAW: create one/use it a lot, GL_DYNAMIC_DRAW: create it a lot/use it a lot, GL_STREAM_DRAW :create it once / use it once)

        glGenBuffers(1, &vbo2);
        glBindBuffer(GL_ARRAY_BUFFER, vbo2);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vert_color), vert_color, GL_STATIC_DRAW);	// copy the data from CPU to GPU (GL_STATIC_DRAW: create one/use it a lot, GL_DYNAMIC_DRAW: create it a lot/use it a lot, GL_STREAM_DRAW :create it once / use it once)

        // The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
        // variables to vertex shaders. in our case - use our only VBO, and say 'every three floats is a variable'
        // Modern OGL requires that we use a vertex array object
        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(GLfloat) * 3), NULL);						// Offset inside the structure to find the attribute
        glEnableVertexAttribArray(0);	// Enable the first attribute or attribute "0" (at index 0)

        // Color attribute, "1"
        glBindBuffer(GL_ARRAY_BUFFER, vbo2);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (sizeof(GLfloat) * 3), NULL);						// Offset inside the structure to find the attribute
        glEnableVertexAttribArray(1);	// Enable the first attribute or attribute "0" (at index 0)

        // 3. Create vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShaderInterleavedSrc, NULL);
        glCompileShader(vs);

        // Check for compile errors
        GLint result;
        GLchar infoLog[512];
        glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Vertex shader failed to compile. %s", infoLog);
            std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
        }

        // 4. Create fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShaderInterleavedSrc, NULL);
        glCompileShader(fs);

        // Check for compile errors
        glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Fragment shader failed to compile. %s", infoLog);
            std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
        }

        // 5. Create shader program and link shaders to program
        GLint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        // Check for linker errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR ) Shader program linker failure. %s", infoLog);
            std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
        }

        // Clean up shaders, do not need them anymore since they are linked to a shader program
        glDeleteShader(vs);
        glDeleteShader(fs);


        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Render the triangle
            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);


            // Swap front and back buffers
            glfwSwapBuffers(gWindow);
        }

        // Clean up
        glDeleteProgram(shaderProgram);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();

        return 0;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return -1;
}
int main_YourDrawingIndexedBuffers(void)
{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            trace.Error("GLFW initialization failed");
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // Set up our quad

        // 1. Set up an array of vertices for a triangle
        //   (What is a vertex?)
        GLfloat list_vertices[] = {
            -0.5f,  0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };
        GLuint indices[] = {
            0, 1, 2,        // Triangle 0
            0, 2, 3,        // Triangle 1
        };

        // 2. Set up buffers on the GPU (vbo = vertex buffer object (identifier), vao = vertex array object (identifier)
        GLuint vbo, ibo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(list_vertices), list_vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU (GL_STATIC_DRAW: create one/use it a lot, GL_DYNAMIC_DRAW: create it a lot/use it a lot, GL_STREAM_DRAW :create it once / use it once)


        // The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
        // variables to vertex shaders. in our case - use our only VBO, and say 'every three floats is a variable'
        // Modern OGL requires that we use a vertex array object
        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(GLfloat) * 3), NULL);						// Offset inside the structure to find the attribute
        glEnableVertexAttribArray(0);	// Enable the first attribute or attribute "0" (at index 0)

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 3. Create vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShaderIndexedSrc, NULL);
        glCompileShader(vs);

        // Check for compile errors
        GLint result;
        GLchar infoLog[512];
        glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Vertex shader failed to compile. %s", infoLog);
            std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
        }

        // 4. Create fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShaderIndexedSrc, NULL);
        glCompileShader(fs);

        // Check for compile errors
        glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR - Fragment shader failed to compile. %s", infoLog);
            std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
        }

        // 5. Create shader program and link shaders to program
        GLint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        // Check for linker errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
            trace.Error("ERROR ) Shader program linker failure. %s", infoLog);
            std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
        }

        // Clean up shaders, do not need them anymore since they are linked to a shader program
        glDeleteShader(vs);
        glDeleteShader(fs);


        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Render the triangle
            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);


            // Swap front and back buffers
            glfwSwapBuffers(gWindow);
        }

        // Clean up
        glDeleteProgram(shaderProgram);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();

        return 0;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return -1;
}

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
//    main_YourFirstTriangle();
//    main_YourFirstTriangleInterleavedColor();
//    main_YourFirstTriangle2Buffers();
    main_YourDrawingIndexedBuffers();
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
    CFuncTracer trace("initOpenGL", m_trace);
    try
    {
        // Intialize GLFW 
        // GLFW is configured.  Must be called before calling any GLFW functions
        if (!glfwInit())
        {
            // An error occured
            std::cerr << "GLFW initialization failed" << std::endl;
            trace.Error("GLFW initialization failed");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

        // Create an OpenGL 3.3 core, forward compatible context window
        gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
        if (gWindow == NULL)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        // Make the window's context the current one
        glfwMakeContextCurrent(gWindow);

        // Initialize GLEW
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            trace.Error("Failed to initialize GLEW");
            return false;
        }

        // Set the required callback functions
        glfwSetKeyCallback(gWindow, glfw_onKey);
        glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);

        glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

        // Define the viewport dimensions
        glfw_onFramebufferSize(gWindow, gWindowWidth, gWindowHeight);

        return true;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return false;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    CFuncTracer trace("glfw_onKey", m_trace);
    try
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            trace.Info("ESC key pressed should stop the mainloop");
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key = GLFW_KEY_W && action == GLFW_PRESS)
        {
            trace.Info("W key pressed, toggle wireframe mode");
            gWireFrame = !gWireFrame;
            if (gWireFrame)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
    CFuncTracer trace("glfw_onFramebufferSize", m_trace);
    try
    {
        glViewport(0, 0, width, height);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Excpetion occured : %s", ex.what());
    }
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

    elapsedSeconds = currentSeconds - previousSeconds;

    // Limit text updates to 4 times per second
    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        // The C++ way of setting the window title
        std::ostringstream outs;
        outs.precision(3);	// decimal places
        outs << std::fixed
            << APP_TITLE << "    "
            << "FPS: " << fps << "    "
            << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        // Reset for next average.
        frameCount = 0;
    }

    frameCount++;
}