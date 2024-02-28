#pragma once
#include "ctracer.h"
#include <memory>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath, std::shared_ptr<CTracer> tracer);
	virtual ~Shader(){}
	
	unsigned int ID(void) { return (unsigned int)m_ID; }
	unsigned long GetLastError(void) { return m_dwError; }
	void Use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

private:
	std::shared_ptr<CTracer> m_trace;
	long m_ID;
	unsigned long m_dwError;
	std::string m_vertexCode;
	std::string m_fragmentCode;

};