#include "SceneBasic_Attributes.h"
#include <cstdio>
#include <cstdlib>
#include <string>
using std::string;
#include <iostream>
using std::cerr;
using std::endl;

#include "GLUtils.h"

SceneBasic_Attrib::SceneBasic_Attrib(std::shared_ptr<CTracer> tracer)
	: Scene(tracer)
{
	CFuncTracer trace("SceneBasic_Attrib::SceneBasic_Attrib", m_trace);
	try
	{

	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
void SceneBasic_Attrib::initScene()
{
	CFuncTracer trace("SceneBasic_Attrib::initScene", m_trace);
	try
	{
		::glDebugMessageCallback(GLUtils::debugCallback, NULL);
		::glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		::glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");

		compileShaderProgram();

		std::cout << std::endl;

		m_prog.printActiveAttribs();

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
		glGenVertexArrays(1, &m_voaHandle);
		glBindVertexArray(m_voaHandle);

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
void SceneBasic_Attrib::update(float t)
{
	CFuncTracer trace("SceneBasic_Attrib::update", m_trace);
	try
	{

	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
void SceneBasic_Attrib::render()
{
	try
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(m_voaHandle);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
	catch (const std::exception&)
	{
	}
}
void SceneBasic_Attrib::resize(int width, int height)
{
	CFuncTracer trace("SceneBasic_Attrib::resize", m_trace);
	try
	{
		m_width = width;
		m_height = m_height;
		::glViewport(0, 0, width, height);
	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
	}
}
void SceneBasic_Attrib::compileShaderProgram()
{
	CFuncTracer trace("SceneBasic_Attrib::compileShaderProgram", m_trace);
	try
	{
		m_prog.compileShader("shaders/basic.vert.glsl");
		m_prog.compileShader("shaders/basic.frag.glsl");
		m_prog.link();
		m_prog.use();
	}
	catch (GLSLProgramException& e)
	{
		trace.Error("GLSLProgram exception : %s", e.what());
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
	catch (const std::exception& ex)
	{
		trace.Error("Exception occurred : %s", ex.what());
		exit(EXIT_FAILURE);
	}
}