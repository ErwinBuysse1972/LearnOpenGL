#include "SceneSamplerObj.h"

#include <memory>
#include <texture.h>
#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

SceneSamplerObj::SceneSamplerObj(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_plane(10.0f, 10.0f, 1, 1)
{
    CFuncTracer trace("SceneSamplerObj::SceneSamplerObj", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneSamplerObj::initScene() 
{
    CFuncTracer trace("SceneSamplerObj::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

        m_view = glm::lookAt(vec3(0.0f, 0.1f, 6.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = mat4(1.0f);

        m_angle = 0.0;

        m_prog.setUniform("Light.L", vec3(1.0f));
        m_prog.setUniform("Light.La", vec3(0.2f));

        // A simple 128x128 checkerboard texture
        GLint w = 128, h = 128, checkSize = 4;
        std::vector<GLubyte> data(w * h * 4);
        for (int r = 0; r < h; ++r)
            for (int c = 0; c < w; ++c)
            {
                GLubyte color = 0;
                if (((c / checkSize) + (r / checkSize)) % 2 == 0)
                    color = 0;
                else
                    color = 255;
                data[(r * w + c) * 4 + 0] = color;
                data[(r * w + c) * 4 + 1] = color;
                data[(r * w + c) * 4 + 2] = color;
                data[(r * w + c) * 4 + 3] = 255;
            }

        // Create the texture object
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
#ifdef __APPLE__
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#else
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
#endif
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

        // Create some sampler objects
        GLuint samplers[2];
        glGenSamplers(2, samplers);
        m_linearSampler = samplers[0];
        m_nearSampler = samplers[1];

        // Set up the nearest sampler
        glSamplerParameteri(m_nearSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(m_nearSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Set up the linear sampler
        glSamplerParameteri(m_linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(m_linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Bind texture object and sampler object to texture unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);

#ifdef __APPLE__
        // Bind the sampler object to the same texture unit and set the sampler uniform
        prog.setUniform("Tex1", 0);
#endif
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSamplerObj::update(float t)
{
    CFuncTracer trace("SceneSamplerObj::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSamplerObj::render()
{
    CFuncTracer trace("SceneSamplerObj::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_prog.setUniform("Light.Position", glm::vec4(0.0f, 20.0f, 0.0f, 1.0f));
        m_prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
        m_prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        m_prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
        m_prog.setUniform("Material.Shininess", 100.0f);

        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(1, 0, 0));

        m_model = glm::translate(rot, glm::vec3(-5.01f, 0.f, 0.f));
        setMatrices();
        glBindSampler(0, m_nearSampler);
        m_plane.render();

        m_model = glm::translate(rot, glm::vec3(5.01f, 0.f, 0.f));
        setMatrices();
        glBindSampler(0, m_linearSampler);
        m_plane.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSamplerObj::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneSamplerObj::renderGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSamplerObj::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneSamplerObj::renderOtherGuiWindow", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSamplerObj::resize(int w, int h)
{
    CFuncTracer trace("SceneSamplerObj::SceneSamplerObj", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSamplerObj::setMatrices(void)
{
    CFuncTracer trace("SceneSamplerObj::SceneSamplerObj", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneSamplerObj::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneSamplerObj::SceneSamplerObj", m_trace);
    try
    {
        m_prog.compileShader("shaders/texture.vert.glsl");
        m_prog.compileShader("shaders/texture.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
