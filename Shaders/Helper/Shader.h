//-----------------------------------------------------------------------------
// ShaderProgram.h by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// GLSL shader manager class
//-----------------------------------------------------------------------------
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include <memory>
#include "cfunctracer.h"
#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"
using std::string;


enum class ShaderType
{
    eVertex,
    eFragment,
    eProgram
};

class ShaderProgram
{
public:
    ShaderProgram(std::shared_ptr<CTracer> tracer);
    ~ShaderProgram();

    // Only supports vertex and fragment (this series will only have those two)
    bool loadShaders(const char* vsFilename, const char* fsFilename);
    void use();

    GLuint getProgram() const;

    void setUniform(const GLchar* name, const glm::vec2& v);
    void setUniform(const GLchar* name, const glm::vec3& v);
    void setUniform(const GLchar* name, const glm::vec4& v);

    void setUniform(const GLchar* name, const glm::mat4& m);

private:
    std::shared_ptr<CTracer>        m_trace;
    GLuint                          m_Handle;
    std::map<string, GLint>         m_UniformLocations;

    string fileToString(const string& filename);
    void  checkCompileErrors(GLuint shader, ShaderType type);
    // We are going to speed up looking for uniforms by keeping their locations in a map
    GLint getUniformLocation(const GLchar* name);


};
#endif // SHADER_H