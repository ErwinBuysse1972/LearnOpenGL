#ifndef HDR_BLOOM_EFFECT_H
#define HDR_BLOOM_EFFECT_H

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <teapot.h>
#include <Sphere.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cfunctracer.h>
#include <memory>

class SceneHDRBloom : public Scene
{
private:
    GLSLProgram         m_prog;
    GLuint              m_fsQuad;
    GLuint              m_hdrFBO;
    GLuint              m_blurFBO;
    GLuint              m_hdrTex;
    GLuint              m_tex1;
    GLuint              m_tex2;
    GLuint              m_linearSampler;
    GLuint              m_nearestSampler;

    Plane               m_plane;
    Teapot              m_teapot;
    Sphere              m_sphere;

    float               m_angle;
    float               m_tPrev;
    float               m_rotspeed;
    int                 m_bloomBufWidth;
    int                 m_bloomBufHeight;

    void setMatrices(void);
    void compileAndLinkShader(void);
    void setupFrameBufferObjects(void);
    void pass1(void);
    void pass2(void);
    void pass3(void);
    void pass4(void);
    void pass5(void);
    float gauss(float, float);
    void drawScene(void);
    void computeLogAveLuminance(void);

public:
    SceneHDRBloom(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif //HDR_BLOOM_EFFECT_H

