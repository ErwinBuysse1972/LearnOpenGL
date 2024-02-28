#include "SceneSeparable.h"

SceneSeparable::SceneSeparable(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
{
    CFuncTracer trace("SceneSeparable::SceneSeparable", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
std::string SceneSeparable::LoadShaderCode(const std::string& file)
{
    CFuncTracer trace("SceneSeparable::SceneSeparable", m_trace);
    try
    {
        // Load contents of file
        std::ifstream inFile(file);
        if (!inFile) {
            std::cerr << "Error opening file: " << file << std::endl;
            exit(EXIT_FAILURE);
        }

        std::stringstream code;
        code << inFile.rdbuf();
        inFile.close();
        std::string codeStr(code.str());

        return codeStr;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return "";
}
void SceneSeparable::CreateShaderPrograms1()
{
    CFuncTracer trace("SceneSeparable::CreateShaderPrograms1", m_trace);
    try
    {
        std::string vertCode = LoadShaderCode("shaders/separable.vert.glsl");
        std::string fragCode1 = LoadShaderCode("shaders/separable1.frag.glsl");
        std::string fragCode2 = LoadShaderCode("shaders/separable2.frag.glsl");

        const GLchar* codePtrs[] = { vertCode.c_str(), fragCode1.c_str(), fragCode2.c_str() };

        m_programs[0] = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, codePtrs);
        CheckLinkStatus(m_programs[0]);
        m_programs[1] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, codePtrs + 1);
        CheckLinkStatus(m_programs[1]);
        m_programs[2] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, codePtrs + 2);
        CheckLinkStatus(m_programs[2]);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSeparable::CreateShaderPrograms2()
{
    CFuncTracer trace("SceneSeparable::CreateShaderPrograms2", m_trace);
    try
    {
        GLuint vertShader = CompileShader("shaders/separable.vert.glsl", GL_VERTEX_SHADER);
        GLuint fragShader1 = CompileShader("shaders/separable1.frag.glsl", GL_FRAGMENT_SHADER);
        GLuint fragShader2 = CompileShader("shaders/separable2.frag.glsl", GL_FRAGMENT_SHADER);

        m_programs[0] = glCreateProgram();
        m_programs[1] = glCreateProgram();
        m_programs[2] = glCreateProgram();

        glProgramParameteri(m_programs[0], GL_PROGRAM_SEPARABLE, GL_TRUE);
        glProgramParameteri(m_programs[1], GL_PROGRAM_SEPARABLE, GL_TRUE);
        glProgramParameteri(m_programs[2], GL_PROGRAM_SEPARABLE, GL_TRUE);

        // Attach the shaders to the program objects
        glAttachShader(m_programs[0], vertShader);
        glAttachShader(m_programs[1], fragShader1);
        glAttachShader(m_programs[2], fragShader2);

        Link(m_programs[0]);
        Link(m_programs[1]);
        Link(m_programs[2]);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSeparable::SetupVao()
{
    CFuncTracer trace("SceneSeparable::SetupVao", m_trace);
    try
    {
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

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSeparable::initScene()
{
    CFuncTracer trace("SceneSeparable::initScene", m_trace);
    try
    {
        // ********************************************************
// Create the separable shader programs.  Call one of these.
// **********************************************************
        //CreateShaderPrograms1();
        CreateShaderPrograms2();

        // **********************************************************
        // Create the two shader pipelines
        // ********************************************************

        // Create two pipelines
        glCreateProgramPipelines(2, m_pipelines);
        // Pipeline 1 uses the vertex shader (programs[0]) and first fragment shader (programs[1])
        glUseProgramStages(m_pipelines[0], GL_VERTEX_SHADER_BIT, m_programs[0]);
        glUseProgramStages(m_pipelines[0], GL_FRAGMENT_SHADER_BIT, m_programs[1]);
        // Pipeline 2 uses the vertex shader (programs[0]) and second fragment shader (programs[2])
        glUseProgramStages(m_pipelines[1], GL_VERTEX_SHADER_BIT, m_programs[0]);
        glUseProgramStages(m_pipelines[1], GL_FRAGMENT_SHADER_BIT, m_programs[2]);

        SetupVao();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSeparable::update(float t)
{
    CFuncTracer trace("SceneSeparable::update", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSeparable::render()
{
    CFuncTracer trace("SceneSeparable::render", m_trace, false);
    try
    {
        glUseProgram(0);

        // Set the uniform
        m_uniLocation = glGetUniformLocation(m_programs[0], "uColorMask");
        glProgramUniform3f(m_programs[0], m_uniLocation, 0.0f, 1.0f, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(m_vaoHandle);

        // Use the first pipeline on the left
        glViewport(0, 0, m_width / 2, m_height);
        glBindProgramPipeline(m_pipelines[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Use the second pipleine on the right
        glViewport(m_width / 2, 0, m_width / 2, m_height);
        glBindProgramPipeline(m_pipelines[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSeparable::resize(int w, int h)
{
    CFuncTracer trace("SceneSeparable::resize", m_trace);
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
void SceneSeparable::Link(GLuint program)
{
    CFuncTracer trace("SceneSeparable::Link", m_trace);
    try
    {    // Link the program
        glLinkProgram(program);
        CheckLinkStatus(program);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSeparable::CheckLinkStatus(GLuint program)
{
    CFuncTracer trace("SceneSeparable::CheckLinkStatus", m_trace);
    try
    {
        // Check for successful linking
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (GL_FALSE == status) {
            std::cerr << "Failed to link shader program!" << std::endl;
            std::cerr << GetProgramInfoLog(program) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
GLuint SceneSeparable::CompileShader(const std::string& file, GLenum shaderType)
{
    CFuncTracer trace("SceneSeparable::CompileShader", m_trace);
    try
    {
        std::string codeStr = LoadShaderCode(file);

        // Create the shader object
        GLuint shader = glCreateShader(shaderType);
        if (0 == shader) {
            std::cerr << "Error creating shader." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Load the source code into the shader object
        const GLchar* codeArray[] = { codeStr.c_str() };
        glShaderSource(shader, 1, codeArray, NULL);

        // Compile the shader
        glCompileShader(shader);

        // Check compilation status
        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (GL_FALSE == result) {
            std::cerr << "Shader compilation failed for " << file << std::endl;
            std::cerr << GetShaderInfoLog(shader) << std::endl;
            exit(EXIT_FAILURE);
        }

        return shader;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    exit(EXIT_FAILURE);
}
std::string SceneSeparable::GetShaderInfoLog(GLuint shader)
{
    CFuncTracer trace("SceneSeparable::GetShaderInfoLog", m_trace);
    try
    {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

        std::string log;
        if (logLen > 0) {
            log.resize(logLen, ' ');
            GLsizei written;
            glGetShaderInfoLog(shader, logLen, &written, &log[0]);
        }

        return log;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return "";
}
std::string SceneSeparable::GetProgramInfoLog(GLuint program)
{
    CFuncTracer trace("SceneSeparable::GetProgramInfoLog", m_trace);
    try
    {
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

        std::string log;
        if (logLen > 0) {
            log.resize(logLen, ' ');
            GLsizei written;
            glGetProgramInfoLog(program, logLen, &written, &log[0]);
        }
        return log;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return "";
}
