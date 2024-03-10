#include "SceneHDRBloom.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


SceneHDRBloom::SceneHDRBloom(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_angle(0.0f)
    , m_tPrev(0.0f)
    , m_bloomBufWidth(0)
    , m_bloomBufHeight(0)
    , m_plane(20.0f, 10.0f, 1, 1)
    , m_teapot(14, glm::mat4(1.0))
    , m_sphere(2.0f, 50, 50)
{
    CFuncTracer trace("SceneHDRBloom::SceneHDRBloom", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::initScene()
{
    CFuncTracer trace("SceneHDRBloom::initScene", m_trace);
    try
    {
        compileAndLinkShader();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        vec3 intense = vec3(0.6f);
        m_prog.setUniform("Lights[0].L", intense);
        m_prog.setUniform("Lights[1].L", intense);
        m_prog.setUniform("Lights[2].L", intense);
        intense = vec3(0.2f);
        m_prog.setUniform("Lights[0].La", intense);
        m_prog.setUniform("Lights[1].La", intense);
        m_prog.setUniform("Lights[2].La", intense);

        m_projection = mat4(1.0f);
        m_angle = glm::pi<float>() / 2.0f;
        setupFrameBufferObjects();

        // Array for full-screen quad
        GLfloat verts[] = {
            -1.0f, -1.0f, 0.0f, 
             1.0f, -1.0f, 0.0f, 
             1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 
             1.0f,  1.0f, 0.0f, 
            -1.0f,  1.0f, 0.0f
        };
        GLfloat tc[] = {
            0.0f, 0.0f, 
            1.0f, 0.0f, 
            1.0f, 1.0f,
            0.0f, 0.0f, 
            1.0f, 1.0f, 
            0.0f, 1.0f
        };

        // Set up the buffers

        unsigned int handle[2];
        glGenBuffers(2, handle);

        glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
        glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

        // Set up the vertex array object

        glGenVertexArrays(1, &m_fsQuad);
        glBindVertexArray(m_fsQuad);

        glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);  // Vertex position

        glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
        glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);  // Texture coordinates

        glBindVertexArray(0);

        m_prog.setUniform("LumThresh", 1.7f);

        float weights[10], sum, sigma2 = 25.0f;

        // Compute and sum the weights
        weights[0] = gauss(0, sigma2);
        sum = weights[0];
        for (int i = 1; i < 10; i++) 
        {
            weights[i] = gauss(float(i), sigma2);
            sum += 2 * weights[i];
        }

        // Normalize the weights and set the uniform
        for (int i = 0; i < 10; i++) 
        {
            std::stringstream uniName;
            uniName << "Weight[" << i << "]";
            float val = weights[i] / sum;
            m_prog.setUniform(uniName.str().c_str(), val);
        }

        // Set up two sampler objects for linear and nearest filtering
        GLuint samplers[2];
        glGenSamplers(2, samplers);
        m_linearSampler = samplers[0];
        m_nearestSampler = samplers[1];

        GLfloat border[] = { 0.0f,0.0f,0.0f,0.0f };
        // Set up the nearest sampler
        glSamplerParameteri(m_nearestSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(m_nearestSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(m_nearestSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glSamplerParameteri(m_nearestSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glSamplerParameterfv(m_nearestSampler, GL_TEXTURE_BORDER_COLOR, border);

        // Set up the linear sampler
        glSamplerParameteri(m_linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(m_linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_linearSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glSamplerParameteri(m_linearSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glSamplerParameterfv(m_linearSampler, GL_TEXTURE_BORDER_COLOR, border);

        // We want nearest sampling except for the last pass.
        glBindSampler(0, m_nearestSampler);
        glBindSampler(1, m_nearestSampler);
        glBindSampler(2, m_nearestSampler);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::update(float t)
{
    CFuncTracer trace("SceneHDRBloom::update", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::render()
{
    CFuncTracer trace("SceneHDRBloom::render", m_trace);
    try
    {
        pass1();
        computeLogAveLuminance();
        pass2();
        pass3();
        pass4();
        pass5();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneHDRBloom::renderGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::renderOtherGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneHDRBloom::renderOtherGuiWindow", m_trace, false);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::resize(int w, int h)
{
    CFuncTracer trace("SceneHDRBloom::resize", m_trace);
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
void SceneHDRBloom::setMatrices(void)
{
    CFuncTracer trace("SceneHDRBloom::setMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::compileAndLinkShader(void)
{
    CFuncTracer trace("SceneHDRBloom::compileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/hdrbloom.vert.glsl");
        m_prog.compileShader("shaders/hdrbloom.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("Exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::setupFrameBufferObjects(void)
{
    CFuncTracer trace("SceneHDRBloom::setupFrameBufferObjects", m_trace);
    try
    {
        // Generate and bind the framebuffer
        glGenFramebuffers(1, &m_hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);

        // Create the texture object
        glGenTextures(1, &m_hdrTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_hdrTex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_width, m_height);

        // Bind the texture to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdrTex, 0);

        // Create the depth buffer
        GLuint depthBuf;
        glGenRenderbuffers(1, &depthBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

        // Bind the depth buffer to the FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

        // Set the targets for the fragment output variables
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);

        // Create an FBO for the bright-pass filter and blur
        glGenFramebuffers(1, &m_blurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);

        // Create two texture objects to ping-pong for the bright-pass filter
        // and the two-pass blur
        m_bloomBufWidth = m_width / 8;
        m_bloomBufHeight = m_height / 8;
        glGenTextures(1, &m_tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_tex1);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_bloomBufWidth, m_bloomBufHeight);

        glActiveTexture(GL_TEXTURE2);
        glGenTextures(1, &m_tex2);
        glBindTexture(GL_TEXTURE_2D, m_tex2);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_bloomBufWidth, m_bloomBufHeight);

        // Bind tex1 to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex1, 0);

        glDrawBuffers(1, drawBuffers);

        // Unbind the framebuffer, and revert to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::pass1(void)
{
    CFuncTracer trace("SceneHDRBloom::pass1", m_trace);
    try
    {
        m_prog.setUniform("Pass", 1);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glViewport(0, 0, m_width, m_height);

        glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(2.0f, 0.0f, 14.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = glm::perspective(glm::radians(60.0f), (float)m_width / m_height, 0.3f, 100.0f);

        drawScene();

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::pass2(void)
{
    CFuncTracer trace("SceneHDRBloom::pass2", m_trace);
    try
    {
        m_prog.setUniform("Pass", 2);

        glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);

        // We're writing to tex1 this time
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex1, 0);

        glViewport(0, 0, m_bloomBufWidth, m_bloomBufHeight);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        m_model = mat4(1.0f);
        m_view = mat4(1.0f);
        m_projection = mat4(1.0f);
        setMatrices();

        // Render the full-screen quad
        glBindVertexArray(m_fsQuad);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::pass3(void)
{
    CFuncTracer trace("SceneHDRBloom::pass3", m_trace);
    try
    {
        m_prog.setUniform("Pass", 3);
        // We're writing to tex2 this time
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex2, 0);

        // Render the full-screen quad
        glBindVertexArray(m_fsQuad);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::pass4(void)
{
    CFuncTracer trace("SceneHDRBloom::pass4", m_trace);
    try
    {
        m_prog.setUniform("Pass", 4);
        // We're writing to tex1 this time
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex1, 0);

        // Render the full-screen quad
        glBindVertexArray(m_fsQuad);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::pass5(void)
{
    CFuncTracer trace("SceneHDRBloom::pass5", m_trace);
    try
    {
        m_prog.setUniform("Pass", 5);

        // Bind to the default framebuffer, this time we're going to
        // actually draw to the screen!
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, m_width, m_height);

        // In this pass, we're reading from tex1 (unit 1) and we want
        // linear sampling to get an extra blur
        glBindSampler(1, m_linearSampler);

        // Render the full-screen quad
        glBindVertexArray(m_fsQuad);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Revert to nearest sampling
        glBindSampler(1, m_nearestSampler);

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
float SceneHDRBloom::gauss(float x, float sigma2)
{
    CFuncTracer trace("SceneHDRBloom::gauss", m_trace);
    try
    {
        double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
        double expon = -(x * x) / (2.0 * sigma2);
        return (float)(coeff * exp(expon));
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return 0.0f;
}
void SceneHDRBloom::drawScene(void)
{
    CFuncTracer trace("SceneHDRBloom::drawScene", m_trace);
    try
    {
        vec3 intense = vec3(1.0f);
        m_prog.setUniform("Lights[0].L", intense);
        m_prog.setUniform("Lights[1].L", intense);
        m_prog.setUniform("Lights[2].L", intense);

        vec4 lightPos = vec4(0.0f, 4.0f, 2.5f, 1.0f);
        lightPos.x = -7.0f;
        m_prog.setUniform("Lights[0].Position", m_view * lightPos);
        lightPos.x = 0.0f;
        m_prog.setUniform("Lights[1].Position", m_view * lightPos);
        lightPos.x = 7.0f;
        m_prog.setUniform("Lights[2].Position", m_view * lightPos);

        m_prog.setUniform("Material.Kd", 0.9f, 0.3f, 0.2f);
        m_prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
        m_prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        m_prog.setUniform("Material.Shininess", 25.0f);

        // The backdrop plane
        m_model = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_plane.render();

        // The bottom plane
        m_model = glm::translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f));
        setMatrices();
        m_plane.render();

        // Top plane
        m_model = glm::translate(mat4(1.0f), vec3(0.0f, 5.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(180.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_plane.render();

        // Sphere
        m_prog.setUniform("Material.Kd", vec3(0.4f, 0.9f, 0.4f));
        m_model = glm::translate(mat4(1.0f), vec3(-3.0f, -3.0f, 2.0f));
        setMatrices();
        m_sphere.render();

        // Teapot
        m_prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.9f));
        m_model = glm::translate(mat4(1.0f), vec3(4.0f, -5.0f, 1.5f));
        m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        m_teapot.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneHDRBloom::computeLogAveLuminance(void)
{
    CFuncTracer trace("SceneHDRBloom::computeLogAveLuminance", m_trace);
    try
    {
        int size = m_width * m_height;
        std::vector<GLfloat> texData(size * 3);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_hdrTex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData.data());
        float sum = 0.0f;
        for (int i = 0; i < size; i++) 
        {
            float lum = glm::dot(vec3(texData[i * 3 + 0], texData[i * 3 + 1], texData[i * 3 + 2]),
                vec3(0.2126f, 0.7152f, 0.0722f));
            sum += logf(lum + 0.00001f);
        }
        m_prog.setUniform("AveLum", expf(sum / size));

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
