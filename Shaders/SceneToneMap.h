#ifndef HDR_LIGHTING_WITH_TONE_MAP_H
#define HDR_LIGHTING_WITH_TONE_MAP_H

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <teapot.h>
#include <Sphere.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneHDRLightToneMap : public Scene
{
private:
    GLSLProgram         m_prog;

    GLuint              m_hdrFBO;
    GLuint              m_quad;
    GLuint              m_hdrTex;
    GLuint              m_avgTex;

    Plane               m_plane;
    Teapot              m_teapot;
    Sphere              m_sphere;

    float               m_angle;
    float               m_tPrev;
    float               m_rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFrameBufferObject();
    void pass1();
    void pass2();
    void computeLogAveLuminance();
    void drawScene();

public:
    SceneHDRLightToneMap(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif //HDR_LIGHTING_WITH_TONE_MAP_H

