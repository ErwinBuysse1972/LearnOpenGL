
#include "Shader.h"
#include "cfunctracer.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, std::shared_ptr<CTracer> tracer)
	: m_ID(-1)
	, m_trace(tracer)
	, m_dwError(0)
	, m_vertexCode("")
	, m_fragmentCode("")
{
	CFuncTracer trace("Shader::Shader", m_trace);
	// 1. retrieve the vertex/fragment source code from filePath
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file’s buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		m_vertexCode = vShaderStream.str();
		m_fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		trace.Error("File read error (%s)", e.what());
	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
void Shader::Use()
{
	CFuncTracer trace("Shader::Use", m_trace);
	try
	{
        //glfwUseProgram(m_ID);
	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
void Shader::SetBool(const std::string& name, bool value) const
{
	CFuncTracer trace("Shader::SetBool", m_trace);
	try
	{

	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
void Shader::SetInt(const std::string& name, int value) const
{
	CFuncTracer trace("Shader::SetInt", m_trace);
	try
	{

	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
void Shader::SetFloat(const std::string& name, float value) const
{
	CFuncTracer trace("Shader::SetFloat", m_trace);
	try
	{

	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
