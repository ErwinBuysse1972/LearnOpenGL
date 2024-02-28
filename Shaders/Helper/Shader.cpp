//-----------------------------------------------------------------------------
// ShaderProgram.cpp by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// GLSL shader manager class
//-----------------------------------------------------------------------------
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
ShaderProgram::ShaderProgram(std::shared_ptr<CTracer> tracer)
    : m_Handle(0)
{
    CFuncTracer trace("ShaderProgram::ShaderProgram", m_trace);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
    CFuncTracer trace("ShaderProgram::~ShaderProgram", m_trace);
    try
    {
        // Delete the program
        if (m_Handle)
            glDeleteProgram(m_Handle);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured:%s", ex.what());
    }
}

//-----------------------------------------------------------------------------
// Loads vertex and fragment shaders
//-----------------------------------------------------------------------------
bool ShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
    CFuncTracer trace("ShaderProgram::loadShaders", m_trace);
    try
    {
        string vsString = fileToString(vsFilename);
        string fsString = fileToString(fsFilename);
        const GLchar* vsSourcePtr = vsString.c_str();
        const GLchar* fsSourcePtr = fsString.c_str();

        // 3. Create vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vs, 1, &vsSourcePtr, NULL);
        glShaderSource(fs, 1, &fsSourcePtr, NULL);

        glCompileShader(vs);
        checkCompileErrors(vs, ShaderType::eVertex);
        glCompileShader(fs);
        checkCompileErrors(fs, ShaderType::eFragment);

        // 5. Create shader program and link shaders to program
        m_Handle = glCreateProgram();
        if (m_Handle == 0)
        {
            std::cerr << "Unable to create shader program!" << std::endl;
            return false;
        }
        glAttachShader(m_Handle, vs);
        glAttachShader(m_Handle, fs);
        glLinkProgram(m_Handle);
        checkCompileErrors(m_Handle, ShaderType::eProgram);

        glAttachShader(m_Handle, vs);
        glAttachShader(m_Handle, fs);

        glLinkProgram(m_Handle);
        checkCompileErrors(m_Handle, ShaderType::eProgram);


        glDeleteShader(vs);
        glDeleteShader(fs);
        m_UniformLocations.clear();
        return true;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return false;
}

//-----------------------------------------------------------------------------
// Opens and reads contents of ASCII file to a string.  Returns the string.
// Not good for very large files.
//-----------------------------------------------------------------------------
string ShaderProgram::fileToString(const string& filename)
{
    CFuncTracer trace("ShaderProgram::fileToString", m_trace);
    std::stringstream ss;
    std::ifstream file;

    try
    {
        file.open(filename, std::ios::in);

        if (!file.fail())
        {
            // Using a std::stringstream is easier than looping through each line of the file
            ss << file.rdbuf();
        }

        file.close();
    }
    catch (std::exception ex)
    {
        std::cerr << "Error reading shader filename!" << std::endl;
        trace.Error("Error reading shader file : %s", ex.what());
    }

    return ss.str();
}

//-----------------------------------------------------------------------------
// Activate the shader program
//-----------------------------------------------------------------------------
void ShaderProgram::use()
{
    CFuncTracer trace("ShaderProgram::use", m_trace);
    try
    {
        if (m_Handle)
            glUseProgram(m_Handle);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

//-----------------------------------------------------------------------------
// Checks for shader compiler errors
//-----------------------------------------------------------------------------
void  ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
    CFuncTracer trace("ShaderProgram::checkCompileErrors", m_trace);
    int status = 0;
    try
    {
        if (type == ShaderType::eProgram)
        {
            glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
            if (status == GL_FALSE)
            {
                GLint length = 0;
                glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

                // The length includes the NULL character
                string errorLog(length, ' ');	// Resize and fill with space character
                glGetProgramInfoLog(m_Handle, length, &length, &errorLog[0]);
                std::cerr << "Error! Shader program failed to link. " << errorLog << std::endl;
                trace.Error("Error! Shader program failed to link. %s", errorLog.c_str());
            }
        }
        else
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE)
            {
                GLint length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

                // The length includes the NULL character
                string errorLog(length, ' ');  // Resize and fill with space character
                glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
                std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
                trace.Error("Error! Shader program failed to compile. %s", errorLog.c_str());
            }
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

//-----------------------------------------------------------------------------
// Returns the active shader program
//-----------------------------------------------------------------------------
GLuint ShaderProgram::getProgram() const
{
    CFuncTracer trace("ShaderProgram::getProgram", m_trace);
    return m_Handle;
}

//-----------------------------------------------------------------------------
// Sets a glm::vec2 shader uniform
//-----------------------------------------------------------------------------
void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
    CFuncTracer trace("ShaderProgram::setUniform", m_trace);
    try
    {
        GLint loc = getUniformLocation(name);
        glUniform2f(loc, v.x, v.y);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}

//-----------------------------------------------------------------------------
// Sets a glm::vec3 shader uniform
//-----------------------------------------------------------------------------
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
    CFuncTracer trace("ShaderProgram::setUniform", m_trace);
    try
    {
        GLint loc = getUniformLocation(name);
        glUniform3f(loc, v.x, v.y, v.z);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

//-----------------------------------------------------------------------------
// Sets a glm::vec4 shader uniform
//-----------------------------------------------------------------------------
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
    CFuncTracer trace("ShaderProgram::setUniform", m_trace);
    try
    {
        GLint loc = getUniformLocation(name);
        glUniform4f(loc, v.x, v.y, v.z, v.w);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}


//-----------------------------------------------------------------------------
// Sets a glm::mat4 shader uniform
//-----------------------------------------------------------------------------
void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m)
{
    CFuncTracer trace("ShaderProgram::setUniform", m_trace);
    try
    {
        GLint loc = getUniformLocation(name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
//-----------------------------------------------------------------------------
// Returns the uniform identifier given it's string name.
// NOTE: Shader must be currently active first.
//-----------------------------------------------------------------------------
GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
    CFuncTracer trace("ShaderProgram::getUniformLocation", m_trace);
    try
    {
        std::map<string, GLint>::iterator it = m_UniformLocations.find(name);
        // Only need to query the shader program IF it doesn't already exist.
        if (it == m_UniformLocations.end())
        {
            // Find it and add it to the map
            m_UniformLocations[name] = glGetUniformLocation(m_Handle, name);
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    // Return it
    return m_UniformLocations[name];
}