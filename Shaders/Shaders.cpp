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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Helper/cfunctracer.h"
#include "Texture2D.h"
#include "Helper/Camera.h"
#include "Shader.h"


// Global Variables
bool gWireFrame = false;
const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Shader";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow* gWindow = NULL;
std::shared_ptr<CTracer> m_trace = nullptr;
const std::string airplaneTxt = "textures/airplane.png";
const std::string crateTxt = "textures/crate.jpg";
const std::string gridTxt = "textures/grid.jpg";
const std::string woodenCrateTxt = "textures/wooden_crate.jpg";

OrbitCamera orbidCamera(m_trace);
float gYaw = 0.0f;
float gPitch = 0.0f;
float gRadius = 10.0f;
const float MOUSE_SENSITIVITY = 0.25f;

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main_CameraII(void)

{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // 1. Set up an array of vectex data for a cube with index buffer data
        GLfloat vertices[] = {
            // position		 // tex coords

           // front face
           -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
           -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
           -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

            // back face
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

             // left face
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
             -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
             -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

             // right face
              1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
              1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
              1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
              1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

              // top face
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
              1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
              1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
              1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

              // bottom face
             -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
              1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
             -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        };

        // Cube position
        glm::vec3 cubePos = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 floorPos = glm::vec3(0.0f, -1.0f, 0.0f);

        // 2. Set up buffers on the GPU
        GLuint vbo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU

        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
        glEnableVertexAttribArray(0);

        // Texture Coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);					// unbind to make sure other code doesn't change it

        ShaderProgram shaderProgram(m_trace);
        shaderProgram.loadShaders("shaders/camera.vert", "shaders/camera.frag");

        Texture2D texture(m_trace);
        texture.loadTexture(woodenCrateTxt, true);

        Texture2D floorTexture(m_trace);
        floorTexture.loadTexture(gridTxt, true);

        double lastTime = glfwGetTime();
        float cubeAngle = 0.0f;

        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture.bind(0);

            glm::mat4 model(1.0), view(1.0), projection(1.0);

            // set the controls of the orbit camera
            orbidCamera.setLookAt(cubePos);
            orbidCamera.rotate(gYaw, gPitch);
            orbidCamera.setRadius(gRadius);

            // Rotates around the cube center
            model = glm::translate(model, cubePos);

            // Create the viewMatrix
            view = orbidCamera.getViewMatrix();

            // Create the projection matrix
            projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

            // Must be called BEFORE setting uniforms because setting uniforms is done
            // on the currently active shader program.
            shaderProgram.use();

            // Pass the matrices to the shader
            shaderProgram.setUniform("model", model);
            shaderProgram.setUniform("view", view);
            shaderProgram.setUniform("projection", projection);

            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            floorTexture.bind(0);

            model = glm::translate(glm::mat4(1.0), floorPos) * glm::scale(glm::mat4(1.0), glm::vec3(10.0f, 0.01f, 10.0f));
            // Send the model matrix for the floor to the vertex shader
            shaderProgram.setUniform("model", model);
            // Draw the floor
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);

            // Swap front and back buffers
            glfwSwapBuffers(gWindow);

            lastTime = currentTime;
        }

        // Clean up
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
int main_CameraI(void)
{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    float gCubeAngle = 0.0f;
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // 1. Set up an array of vectex data for a cube with index buffer data
        GLfloat vertices[] = {
            // position		 // tex coords

           // front face
           -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
           -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
           -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

            // back face
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

             // left face
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
             -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
             -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

             // right face
              1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
              1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
              1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
              1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

              // top face
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
              1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
              1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
             -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
              1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

              // bottom face
             -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
              1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
             -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
              1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        };

        // Cube position
        glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -12.0f);

        // 2. Set up buffers on the GPU
        GLuint vbo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU

        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
        glEnableVertexAttribArray(0);

        // Texture Coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);					// unbind to make sure other code doesn't change it

        ShaderProgram shaderProgram(m_trace);
        shaderProgram.loadShaders("shaders/camera.vert", "shaders/camera.frag");

        Texture2D texture(m_trace);
        texture.loadTexture(woodenCrateTxt, true);

        double lastTime = glfwGetTime();
        float cubeAngle = 0.0f;

        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture.bind(0);

            glm::mat4 model(1.0), view(1.0), projection(1.0);

            // Update the cube position and orientation.  Rotate first then translate
            cubeAngle += (float)(deltaTime * 50.0f);
            if (cubeAngle >= 360.0f) cubeAngle = 0.0f;

            // Rotates around the cube center
            model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
//            model = glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model, cubePos);

            // Uncomment this line and comment the one above to flip the transformation of the cube.  Watch what happens!
            // Rotates around??
            //model = glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model, cubePos);

            glm::vec3 camPos(0.0f, 0.0f, 0.0f);
            glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
            glm::vec3 up(0.0f, 1.0f, 0.0f);

            // Create the View matrix
            view = glm::lookAt(camPos, camPos + targetPos, up);

            // Create the projection matrix
            projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

            // Must be called BEFORE setting uniforms because setting uniforms is done
            // on the currently active shader program.
            shaderProgram.use();

            // Pass the matrices to the shader
            shaderProgram.setUniform("model", model);
            shaderProgram.setUniform("view", view);
            shaderProgram.setUniform("projection", projection);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            // Swap front and back buffers
            glfwSwapBuffers(gWindow);

            lastTime = currentTime;
        }

        // Clean up
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
int main_TextureMapping(void)

{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // Set up our quad

        // 1. Set up an array of vertices for a quad (2 triangls) with an index buffer data
        //   (What is a vertex?)
        GLfloat vertices[] = {
            // position             
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,         // top left		
             0.5f,  0.5f, 0.0f,	1.0f, 1.0f,         // top right
             0.5f, -0.5f, 0.0f,	1.0f, 0.0f,         // bottom left 
           - 0.5f, -0.5f, 0.0f,	0.0f, 0.0f,         // bottom right
        };

        GLuint indices[] = {
            0, 1, 2,  // First Triangle
            0, 2, 3   // Second Triangle
        };

        // 2. Set up buffers on the GPU
        GLuint vbo, ibo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU

        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one, all attributes that follow here are related to VOA object
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);	// Define a layout for the first vertex buffer "0" -> voa object
        // (look main_YourFirstTriangle2Buffers example using multiple buffers)
        glEnableVertexAttribArray(0);			// Enable the first attribute or attribute "0"

        // tex coord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Set up index buffer
        glGenBuffers(1, &ibo);	// Create buffer space on the GPU for the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);					// unbind to make sure other code doesn't change it

        ShaderProgram shaderProgram(m_trace);
        shaderProgram.loadShaders("shaders/Texture.vert", "shaders/Texture.frag");

        Texture2D texture(m_trace);
        texture.loadTexture(airplaneTxt, true);

        Texture2D texture1(m_trace);
        texture1.loadTexture(crateTxt, true);

        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            texture.bind(0);
            texture1.bind(1);

            // Render the quad
            // Must be called BEFORE setting uniforms because setting uniforms is done
            // on the currently active shader program.
            shaderProgram.use();
            glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture1"), 0);
            glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture2"), 1);

            // Setting uniforms should be done between .use function and drawing
            GLfloat time = (GLfloat)glfwGetTime();// returns time in sec since start
            GLfloat blueColor = (sin(time) / 2) + 0.5f;
            glm::vec2 pos;
            pos.x = cos(time) / 2;
            pos.y = sin(time) / 2;

            shaderProgram.setUniform("vertColor", glm::vec4(0.0f, 0.0f, blueColor, 1.0f));
            shaderProgram.setUniform("posOffset", pos);

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Swap front and back buffers
            glfwSwapBuffers(gWindow);
        }

        // Clean up
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);

        glfwTerminate();

        return 0;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return -1;
}
int main_shadersUsingUniform(void)
{
    m_trace = std::make_shared<CFileTracer>("", "YourFirstTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_YourFirstTriangle", m_trace);
    try
    {
        if (!initOpenGL())
        {
            // An error occured
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // Set up our quad

        // 1. Set up an array of vertices for a quad (2 triangls) with an index buffer data
        //   (What is a vertex?)
        GLfloat vertices[] = {
            -0.5f,  0.5f, 0.0f,		// Top left
             0.5f,  0.5f, 0.0f,		// Top right
             0.5f, -0.5f, 0.0f,		// Bottom right
            -0.5f, -0.5f, 0.0f		// Bottom left 
        };

        GLuint indices[] = {
            0, 1, 2,  // First Triangle
            0, 2, 3   // Second Triangle
        };

        // 2. Set up buffers on the GPU
        GLuint vbo, ibo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU

        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one, all attributes that follow here are related to VOA object
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// Define a layout for the first vertex buffer "0" -> voa object
                                                                    // (look main_YourFirstTriangle2Buffers example using multiple buffers)
        glEnableVertexAttribArray(0);			// Enable the first attribute or attribute "0"

        // Set up index buffer
        glGenBuffers(1, &ibo);	// Create buffer space on the GPU for the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);					// unbind to make sure other code doesn't change it

        ShaderProgram shaderProgram(m_trace);
        shaderProgram.loadShaders("shaders/uniform.vert", "shaders/uniform.frag");

        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Render the quad
            // Must be called BEFORE setting uniforms because setting uniforms is done
            // on the currently active shader program.
            shaderProgram.use();

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Swap front and back buffers
            glfwSwapBuffers(gWindow);
        }

        // Clean up
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);

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
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
        }

        // Set up our quad

        // 1. Set up an array of vertices for a quad (2 triangls) with an index buffer data
        //   (What is a vertex?)
        GLfloat vertices[] = {
            -0.5f,  0.5f, 0.0f,		// Top left
             0.5f,  0.5f, 0.0f,		// Top right
             0.5f, -0.5f, 0.0f,		// Bottom right
            -0.5f, -0.5f, 0.0f		// Bottom left 
        };

        GLuint indices[] = {
            0, 1, 2,  // First Triangle
            0, 2, 3   // Second Triangle
        };

        // 2. Set up buffers on the GPU
        GLuint vbo, ibo, vao;

        glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU

        glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
        glBindVertexArray(vao);					// Make it the current one
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);	// Define a layout for the first vertex buffer "0"
        glEnableVertexAttribArray(0);			// Enable the first attribute or attribute "0"

        // Set up index buffer
        glGenBuffers(1, &ibo);	// Create buffer space on the GPU for the index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);					// unbind to make sure other code doesn't change it

        ShaderProgram shaderProgram(m_trace);
        shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");
        // Rendering loop
        while (!glfwWindowShouldClose(gWindow))
        {
            showFPS(gWindow);

            // Poll for and process events
            glfwPollEvents();

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Render the quad
            // Must be called BEFORE setting uniforms because setting uniforms is done
            // on the currently active shader program.
            shaderProgram.use();
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Swap front and back buffers
            glfwSwapBuffers(gWindow);
        }

        // Clean up
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);

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
#if 0
int main(int argc, char* argv[])
{
//    main_YourDrawingIndexedBuffers();
  //  main_shadersUsingUniform();
   // main_TextureMapping();
    //main_CameraI();
    main_CameraII();
}
#endif

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
        glfwSetCursorPosCallback(gWindow, glfw_onMouseMove);

        glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

        // Define the viewport dimensions
        glViewport(0, 0, gWindowWidth, gWindowHeight);
        glEnable(GL_DEPTH_TEST);

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

        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
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
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY)
{
    CFuncTracer trace("glfw_onMouseMove", m_trace);
    static glm::vec2 lastMousePos = glm::vec2(0, 0);
    try
    {
        // update the angles based on the left mouse button input to orbit around the cube
        if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1)
        {
            // each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
            gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
            gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
        }

        // change the orbid camera radius with the right mouse button
        if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1)
        {
            float dx = 0.1f * ((float)posX - lastMousePos.x);
            float dy = 0.1f * ((float)posY - lastMousePos.y);
            gRadius += (dx - dy);
        }

        lastMousePos.x = (float)posX;
        lastMousePos.y = (float)posY;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
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
        gWindowWidth = width;
        gWindowHeight = height;
        glViewport(0, 0, gWindowWidth, gWindowHeight);

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
