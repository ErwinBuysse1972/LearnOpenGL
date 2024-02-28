#include <iostream>
#include <vector>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// normalized vertices of a rectangle
float vertices_triangle[] =
{
	// x,   y,    z
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

float vertices_two_triangle[] =
{
    //x    
    0.5f,  0.5f, 0.0f,  // top right 
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f,  // top left
};

int indices[] =
{
    0,1,3,     // first trianlge
    1,2,3      // second triangle
};

//shader code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
"}\0";

void ShowFPS(GLFWwindow* window, const std::string& title)
{
    static double previousSeconds = 0.0f;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime();

    elapsedSeconds = currentSeconds - previousSeconds;
    // update the title 4 times per ssecond
    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << title << "    "
            << "FPS: " << fps << "    "
            << "Frame time: " << msPerFrame << " ms.";
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }
    frameCount++;
}

void framebuffer_size_cb(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* initialize_createWindow(int width, int height, const std::string& title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
    return window;
}
long compile_shader(const char* code, unsigned shaderType)
{
    unsigned int shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    // checks if the compilation succeeded
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX_SHADER::COMPILATION::FAILED (" << infoLog << ")" << std::endl;
        return -1;
    }
    return (long)shader;
}
long linkProgram(std::vector<unsigned int> shaders)
{
    //Links the shaders in one shader program
    int success;
    char infoLog[512];
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    for (auto& shader : shaders)
        glAttachShader(shaderProgram, shader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION::FAILED (" << infoLog << ")" << std::endl;
        shaderProgram = -1;
    }

    for (auto& shader : shaders)
        glDeleteShader(shader);

    return shaderProgram;
}


void HelloWindow(void)
{
    GLFWwindow* window = initialize_createWindow(800, 600, "LearningOpenGL");
    if (window == nullptr)
    {
        std::cerr << "Failed to create and initialize window (LearningOpenGL)" << std::endl;
        return;
    }
#if 1
	// create the vertex shader, nothing is transformed the input is equal to the output
	long vertexShader = compile_shader(vertexShaderSource, GL_VERTEX_SHADER);
    if ((int)vertexShader < 0)
    {
        std::cerr << "Failed to create the vertex shader" << std::endl;
        return;
    }

	// creates the fragment shader
	long fragmentShader =  compile_shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    if ((long)fragmentShader < 0)
    {
        std::cerr << "Failed to create the vertex shader" << std::endl;
        return;
    }
    
    //Links the shaders in one shader program
    std::vector<unsigned int> shaders = { (unsigned int)vertexShader, (unsigned int)fragmentShader };
    long shaderProgram = linkProgram(shaders);
    if (shaderProgram < 0)
    {
        std::cerr << "Failed to link program" << std::endl;
        return;
    }

    // create the Vertex Buffer object (VBO) 
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // create Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
    
    //copy the vertices into the GPU memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
#endif
	while (!glfwWindowShouldClose(window))
	{
        ShowFPS(window, "LearningOpenGL");

		// process input keys
		process_input(window);

		// Prepare the rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
#if 1
		// Render generated VAO's (objects)
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
#endif
        //Swap buffers and render display the next frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
void Draw2TrianglesUsingEBO_VAO(void)
{
    GLFWwindow* window = initialize_createWindow(800, 600, "LearningOpenGL");
    if (window == nullptr)
    {
        std::cerr << "Failed to create and initialize window (LearningOpenGL)" << std::endl;
        return;
    }
    // create the vertex shader, nothing is transformed the input is equal to the output
    long vertexShader = compile_shader(vertexShaderSource, GL_VERTEX_SHADER);
    if ((int)vertexShader < 0)
    {
        std::cerr << "Failed to create the vertex shader" << std::endl;
        return;
    }

    // creates the fragment shader
    long fragmentShader = compile_shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    if ((long)fragmentShader < 0)
    {
        std::cerr << "Failed to create the vertex shader" << std::endl;
        return;
    }

    //Links the shaders in one shader program
    std::vector<unsigned int> shaders = { (unsigned int)vertexShader, (unsigned int)fragmentShader };
    long shaderProgram = linkProgram(shaders);
    if (shaderProgram < 0)
    {
        std::cerr << "Failed to link program" << std::endl;
        return;
    }

    //create buffers in GPU
    unsigned int VBO, EBO, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    
    //  Bind all the different buffers into VAO
    glBindVertexArray(VAO);
    //  copy the vertices into the GPU memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_two_triangle), vertices_two_triangle, GL_STATIC_DRAW);
    //  copy the indices to GPU memory
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //  Add vertex attributes to VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render kiio
    while (!glfwWindowShouldClose(window))
    {
        ShowFPS(window, "LearningOpenGL");
        // process input neys
        process_input(window);

        // Prepare the rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render generated VAO's (objects)
        glUseProgram(shaderProgram);

        // set the VAO inside the context
        glBindVertexArray(VAO);
        //  draw VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // release VAO from context
        glBindVertexArray(0);
        //Swap buffers and render display the next frame
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
}
int main(int argc, char* argv[])
{
	//HelloWindow();
    Draw2TrianglesUsingEBO_VAO();
	return 0;
}