#ifndef SCENE_GAUSSIAN_BLUR_FILTER_H
#define SCENE_GAUSSIAN_BLUR_FILTER_H

#include <memory>

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <Cube.h>
#include <Torus.h>
#include <teapot.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneGausianBlur : public Scene
{
private:
    GLSLProgram         m_prog;
    GLuint              m_fsQuad;
    GLuint              m_renderFBO;
    GLuint              m_intermediateFBO;
    GLuint              m_renderTex;
    GLuint              m_intermediateTex;

    Plane               m_plane;
    Torus               m_torus;
    Teapot              m_teapot;
    Cube                m_cube;

    float               m_angle;
    float               m_tPrev;
    float               m_rotSpeed;

    void setMatrices(void);
    void compileAndLinkShader(void);
    void setupFrameBufferObjects(void);
    void pass1(void);
    void pass2(void);
    void pass3(void);
    float gaus(float, float);

public:
    SceneGausianBlur(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif // SCENE_GAUSSIAN_BLUR_FILTER_H

